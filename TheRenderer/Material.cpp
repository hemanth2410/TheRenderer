#include "Material.h"
#include "BindableCommon.h"
#include "DynamicConstant.h"
#include "ConstantBuffersEx.h"
//#include "TransformCbufScaling.h"
#include "Stencil.h"
#include <filesystem>


void Material::SetMaterialToUse(MaterialToUse _InShader)
{
	materialToUse = _InShader;
}



Material::Material(Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path) noxnd
	:
modelPath(path.string()),
materialToUse(MaterialToUse::PBR_Metallic_Roughness)
{
	using namespace Bind;
	const auto rootPath = path.parent_path().string() + "\\";
	{
		aiString tempName;
		material.Get(AI_MATKEY_NAME, tempName);
		name = tempName.C_Str();
	}

	bool hasTexture = false;
	bool hasORMtexture = false;
	bool hasAlpha = false;
	bool hasGlossAlpha = false;
	bool hasNormalMap = false;
	aiString texFileName;
	Step step("lambertian");
	std::string shaderCode = "";
	Dcb::RawLayout pscLayout;



	// since we refactored the mesh code now it's not possible to implement the thader to use in the mesh class now we have to do it in the material itself. 
	// if we have GLTF_METALLIC_ROUGHNESS then its pbr if it has SPECULAR MAP then its phong shading.
	if (material.GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &texFileName) == aiReturn_SUCCESS)
	{
		materialToUse = MaterialToUse::PBR_Metallic_Roughness;
	}
	else if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
	{
		materialToUse = MaterialToUse::phong;
	}
	else 
	{
		// -> This has none may be fall back to solid??? 
	}
	Technique phongShading{ "Phong" };
	Technique PBR{ "PBR_Metal_Roughness" };
	switch (materialToUse)
	{
	case PBR_Metallic_Roughness:
		shaderCode = "PBR_Metal_Roughness";

		//initialize the buffers
		pscLayout.Add<Dcb::Float>("specularIntensity");
		pscLayout.Add<Dcb::Float>("specularPower");
		pscLayout.Add<Dcb::Bool>("useSpecularMap");
		pscLayout.Add<Dcb::Bool>("useNormalMap");
		pscLayout.Add<Dcb::Float>("metallicFactor");
		pscLayout.Add<Dcb::Float>("roughnessFactor");
		pscLayout.Add<Dcb::Float>("ambientFactor");
		pscLayout.Add<Dcb::Bool>("ormMap");
		
		// common (pre)
		vtxLayout.Append(Dvtx::VertexLayout::Position3D);
		vtxLayout.Append(Dvtx::VertexLayout::Normal);
		//diffuse
		{
			hasAlpha = false;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Dif";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str());
				if (tex->HasAlpha())
				{
					hasAlpha = true;
					shaderCode += "Msk";
				}
				step.AddBindable(std::move(tex));
			}
			else
			{
				//pscLayout.Add<Dcb::Float3>("materialColor");
			}
			step.AddBindable(Rasterizer::Resolve(gfx, hasAlpha));
		}
		//ormMap
		{
			if (material.GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasORMtexture = true;
				shaderCode += "ORM";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 3);
				step.AddBindable(std::move(tex));
				//pscLayout.Add<Dcb::Bool>("ormMap");
			}
			//pscLayout.Add<Dcb::Float>("metallicFactor");
			//pscLayout.Add<Dcb::Float>("roughnessFactor");
			//pscLayout.Add<Dcb::Float>("ambientFactor");
		}
		// normal
		{

			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Nrm";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				step.AddBindable(Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2));
				//pscLayout.Add<Dcb::Bool>("useNormalMap");
			}
			else 
			{
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
			}

			vtxLayout.Append(Dvtx::VertexLayout::Tangent);
			vtxLayout.Append(Dvtx::VertexLayout::Bitangent); // <- append these even when you dont have the normal map
		}
		// common (post)
		{
			step.AddBindable(std::make_shared<TransformCbuf>(gfx, 0u));
			step.AddBindable(Blender::Resolve(gfx, false));
			auto pvs = VertexShader::Resolve(gfx, "PhongShadingVS_NRML.cso");
			auto pvsbc = pvs->GetBytecode();
			step.AddBindable(std::move(pvs));
			step.AddBindable(PixelShader::Resolve(gfx, hasAlpha ? "PBR_Metal_Roughness_AlphaMask.cso" : "PBR_Metal_Roughness.cso"));
			step.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));
			if (hasTexture)
			{
				step.AddBindable(Bind::Sampler::Resolve(gfx));
			}
			// PS material params (cbuf)
			Dcb::Buffer buf{ std::move(pscLayout) };
			buf["specularIntensity"] = 1.0f;   // unused by PBR shader, but must exist for offsets to line up
			buf["specularPower"] = 30.0f;
			buf["useSpecularMap"] = false;
			buf["useNormalMap"].SetIfExists(true);
			buf["metallicFactor"].SetIfExists(0.0f);
			buf["roughnessFactor"].SetIfExists(0.5f);
			buf["ambientFactor"].SetIfExists(0.0f);
			buf["ormMap"].SetIfExists(hasORMtexture);
			//buf["normalMapWeight"].SetIfExists(1.0f);
			step.AddBindable(std::make_unique<Bind::CachingPixelConstantBufferEx>(gfx, std::move(buf), 1u));
		}
		PBR.AddStep(std::move(step));
		techniques.push_back(std::move(PBR));
		break;
	case phong:
		shaderCode = "Phong";
		// common (pre)
		vtxLayout.Append(Dvtx::VertexLayout::Position3D);
		vtxLayout.Append(Dvtx::VertexLayout::Normal);
		// diffuse
		{
			bool hasAlpha = false;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Dif";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str());
				if (tex->HasAlpha())
				{
					hasAlpha = true;
					shaderCode += "Msk";
				}
				step.AddBindable(std::move(tex));
			}
			else
			{
				pscLayout.Add<Dcb::Float3>("materialColor");
			}
			step.AddBindable(Rasterizer::Resolve(gfx, hasAlpha));
		}
		// specular
		{
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Spc";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 1);
				hasGlossAlpha = tex->HasAlpha();
				step.AddBindable(std::move(tex));
				pscLayout.Add<Dcb::Bool>("useGlossAlpha");
				pscLayout.Add<Dcb::Bool>("useSpecularMap");
			}
			pscLayout.Add<Dcb::Float3>("specularColor");
			pscLayout.Add<Dcb::Float>("specularWeight");
			pscLayout.Add<Dcb::Float>("specularGloss");
		}
		// normal
		{
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Nrm";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				vtxLayout.Append(Dvtx::VertexLayout::Tangent);
				vtxLayout.Append(Dvtx::VertexLayout::Bitangent);
				step.AddBindable(Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2));
				pscLayout.Add<Dcb::Bool>("useNormalMap");
				pscLayout.Add<Dcb::Float>("normalMapWeight");
			}
		}
		// common (post)
		{
			step.AddBindable(std::make_shared<TransformCbuf>(gfx, 0u));
			step.AddBindable(Blender::Resolve(gfx, false));
			auto pvs = VertexShader::Resolve(gfx, "PhongShadingVS_NRML.cso");
			auto pvsbc = pvs->GetBytecode();
			step.AddBindable(std::move(pvs));
			step.AddBindable(PixelShader::Resolve(gfx, "PhongShadingNS_PS.cso"));
			step.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));
			if (hasTexture)
			{
				step.AddBindable(Bind::Sampler::Resolve(gfx));
			}
			// PS material params (cbuf)
			Dcb::Buffer buf{ std::move(pscLayout) };
			if (auto r = buf["materialColor"]; r.Exists())
			{
				aiColor3D color = { 0.45f,0.45f,0.85f };
				material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
				r = reinterpret_cast<DirectX::XMFLOAT3&>(color);
			}
			buf["useGlossAlpha"].SetIfExists(hasGlossAlpha);
			buf["useSpecularMap"].SetIfExists(true);
			if (auto r = buf["specularColor"]; r.Exists())
			{
				aiColor3D color = { 0.18f,0.18f,0.18f };
				material.Get(AI_MATKEY_COLOR_SPECULAR, color);
				r = reinterpret_cast<DirectX::XMFLOAT3&>(color);
			}
			buf["specularWeight"].SetIfExists(1.0f);
			if (auto r = buf["specularGloss"]; r.Exists())
			{
				float gloss = 8.0f;
				material.Get(AI_MATKEY_SHININESS, gloss);
				r = gloss;
			}
			buf["useNormalMap"].SetIfExists(true);
			buf["normalMapWeight"].SetIfExists(1.0f);
			step.AddBindable(std::make_unique<Bind::CachingPixelConstantBufferEx>(gfx, std::move(buf), 1u));
		}
		phongShading.AddStep(std::move(step));
		techniques.push_back(std::move(phongShading));
		break;
	case solid:
		break;
	default:
		break;
	}

	// outline technique
	{
		Technique outline("Outline", false);
		{
			Step mask("outlineMask");

			auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
			auto pvsbc = pvs->GetBytecode();
			mask.AddBindable(std::move(pvs));

			// TODO: better sub-layout generation tech for future consideration maybe
			mask.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));

			mask.AddBindable(std::make_shared<TransformCbuf>(gfx));

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep(std::move(mask));
		}
		{
			Step draw("outlineDraw");

			// these can be pass-constant (tricky due to layout issues)
			auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
			auto pvsbc = pvs->GetBytecode();
			draw.AddBindable(std::move(pvs));

			// this can be pass-constant
			draw.AddBindable(PixelShader::Resolve(gfx, "SolidPS.cso"));

			{
				Dcb::RawLayout lay;
				lay.Add<Dcb::Float4>("color");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = DirectX::XMFLOAT4{ 1.0f,0.4f,0.4f,1.0f };
				draw.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u));
			}
			// TODO: better sub-layout generation tech for future consideration maybe
			draw.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));




			draw.AddBindable(std::make_shared<TransformCbuf>(gfx));

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep(std::move(draw));
		}
		techniques.push_back(std::move(outline));
	}
}
Dvtx::VertexBuffer Material::ExtractVertices(const aiMesh& mesh) const noexcept
{
	return { vtxLayout,mesh };
}
std::vector<unsigned short> Material::ExtractIndices(const aiMesh& mesh) const noexcept
{
	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	return indices;
}
std::shared_ptr<Bind::VertexBuffer> Material::MakeVertexBindable(Graphics& gfx, const aiMesh& mesh, float scale) const noxnd
{
	auto vtc = ExtractVertices(mesh);
	if (scale != 1.0f)
	{
		for (auto i = 0u; i < vtc.Size(); i++)
		{
			DirectX::XMFLOAT3& pos = vtc[i].Attr<Dvtx::VertexLayout::ElementType::Position3D>();
			pos.x *= scale;
			pos.y *= scale;
			pos.z *= scale;
		}
	}
	return Bind::VertexBuffer::Resolve(gfx, MakeMeshTag(mesh), std::move(vtc));
}
std::shared_ptr<Bind::IndexBuffer> Material::MakeIndexBindable(Graphics& gfx, const aiMesh& mesh) const noxnd
{
	return Bind::IndexBuffer::Resolve(gfx, MakeMeshTag(mesh), ExtractIndices(mesh));
}
std::string Material::MakeMeshTag(const aiMesh& mesh) const noexcept
{
	return modelPath + "%" + mesh.mName.C_Str();
}
std::vector<Technique> Material::GetTechniques() const noexcept
{
	return techniques;
}