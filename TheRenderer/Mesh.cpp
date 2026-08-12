#include "Mesh.h"
#include "imgui/imgui.h"
#include <unordered_map>
#include <sstream>
#include "GeometryMath.h"
#include "Transform.h"
#include "GameCoordinates.h"
#include "Surface.h"
#include "TransformCbufDoubleboi.h"
namespace dx = DirectX;

ModelException::ModelException(int line, const char* file, std::string note) noexcept
	:
	ChiliException(line, file),
	note(std::move(note))
{}

const char* ModelException::what() const noexcept
{
	std::ostringstream oss;
	oss << ChiliException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
	return "Chili Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
	return note;
}

// Mesh
Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
	//if (!IsStaticInitialized())
	//{
	//	AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	//}
	//addBind(std::make_shared<Bind::Topology>(gfx, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	addBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	for (auto& pb : bindPtrs)
	{
		//if (auto pi = dynamic_cast<Bind::IndexBuffer*>(pb.get()))
		//{
		//	AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer>{ pi });
		//	pb.release();
		//}
		//else
		//{
		//	AddBind(std::move(pb));
		//}
		addBind(std::move(pb));
	}

	//addBind(std::make_shared<Bind::TransformCbuf>(gfx, *this));
	//addBind(std::make_shared<transformcbufdouble>(gfx, *this, 0u, 2u));
	addBind(std::make_shared<Bind::TransformCbufDoubleboi>(gfx, *this, 0u, 2u));
}
void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(gfx);
}
DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}



// Node
Node::Node(int id, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& _transform, std::string name) noxnd
	:
id(id),
meshPtrs(std::move(meshPtrs)),
name(name)
{
	DirectX::XMStoreFloat4x4(&transform, _transform);
	DirectX::XMStoreFloat4x4(&appliedTransform, dx::XMMatrixIdentity());
}
void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd
{
	const auto built = 
		DirectX::XMLoadFloat4x4(&appliedTransform) *
		DirectX::XMLoadFloat4x4(&transform)* 
		accumulatedTransform;
	for (const auto pm : meshPtrs)
	{
		pm->Draw(gfx, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(gfx, built);
	}
}
void Node::AddChild(std::unique_ptr<Node> pChild) noxnd
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}
void Node::ShowTree(Node*& pSelectedNode) const noexcept
{
	/*const int currentNodeIndex = nodeIndexTracked;
	nodeIndexTracked++;*/
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow 
		| ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	const auto expanded = (ImGui::TreeNodeEx((void*)(intptr_t)GetId(), node_flags, name.c_str()));
		if (ImGui::IsItemClicked())
		{
			//selectedIndex = GetId();
			pSelectedNode = const_cast<Node*>(this);
		}
		/*for (const auto& pm : meshPtrs)
		{
			ImGui::Text("Mesh");
		}*/
		if (expanded)
		{
			for (const auto& pc : childPtrs)
			{
				pc->ShowTree(pSelectedNode);
			}
			ImGui::TreePop();
		}
		
}
void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
	dx::XMStoreFloat4x4(&appliedTransform, transform);
}
std::string Node::GetName()noexcept
{
	return name;
}
int Node::GetId() const noexcept
{
	return id;
}
// Model

class ModelWindow
{
public:
	void Show(const char* windowName, const Node& root) noexcept
	{
		windowName = windowName ? windowName : "Model Hierarchy";
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName)) {
			//ImGui::Columns(2, nullptr, true);
			root.ShowTree(pSelectedNode);
			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[pSelectedNode->GetId()];
				//ImGui::NextColumn();
				//ImGui::Text("Transform");
				//ImGui::InputFloat3("Position", transform.position);
				//ImGui::InputFloat3("Rotation", transform.rotation);
				//ImGui::InputFloat3("Scale", transform.scale);
				///*ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				//ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				//ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);*/
				///*ImGui::Text("Position");
				//ImGui::SliderFloat("X", &transform.posX, -20.0f, 20.0f);
				//ImGui::SliderFloat("Y", &transform.posY, -20.0f, 20.0f);
				//ImGui::SliderFloat("Z", &transform.posZ, -20.0f, 20.0f);
				//ImGui::Text("Scale");
				//ImGui::SliderFloat("X##Scale", &transform.scaleX, 0.01f, 2.0f, "%.2f", 2.0f);
				//ImGui::SliderFloat("Y##Scale", &transform.scaleY, 0.01f, 2.0f, "%.2f", 2.0f);
				//ImGui::SliderFloat("Z##Scale", &transform.scaleZ, 0.01f, 2.0f, "%.2f", 2.0f);*/
				//ImGui::LabelText("##ScaleLabel", "Unit Scale Factor : %f", scale);

				// custom stuff

				Transform t;
				t.SetPosition(transform.position[0], transform.position[1], transform.position[2]);
				// existing mapping (see GetTransform() below): rotation[0]=pitch, rotation[1]=roll, rotation[2]=yaw
				t.SetRotationEuler(
					to_rad(transform.rotation[0]),
					to_rad(transform.rotation[2]),
					to_rad(transform.rotation[1])
				);
				t.SetScale(transform.scale[0], transform.scale[1], transform.scale[2]);

				transformWindow.SpawnTransformWindow(t, pSelectedNode->GetName().c_str());

				// write back any edits made in the new window into the existing float arrays
				transform.position[0] = t.position.x;
				transform.position[1] = t.position.y;
				transform.position[2] = t.position.z;
				const Vector3 euler = t.rotation.ToEuler(); // .x=pitch .y=yaw .z=roll
				transform.rotation[0] = to_deg(euler.x);
				transform.rotation[2] = to_deg(euler.y);
				transform.rotation[1] = to_deg(euler.z);
				transform.scale[0] = t.scale.x;
				transform.scale[1] = t.scale.y;
				transform.scale[2] = t.scale.z;
			}
			//root.ShowTree(nodeIndexTracker, selectedIndex);
			
		}
		ImGui::End();


	}
	dx::XMMATRIX GetTransform() const noexcept {
		if (pSelectedNode == nullptr) {
			return DirectX::XMMatrixIdentity();
		}
		const auto& transform = transforms.at(pSelectedNode->GetId());
		return
			DirectX::XMMatrixRotationRollPitchYaw(to_rad(transform.rotation[0]), to_rad(transform.rotation[2]), to_rad(transform.rotation[1])) *
			DirectX::XMMatrixTranslation(transform.position[0], transform.position[1], transform.position[2]) *
			DirectX::XMMatrixScaling(transform.scale[0], transform.scale[1], transform.scale[2]);
	}
	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}
	void SetScaleFactor(float inScale)
	{
		scale = inScale;
	}
private:
	//std::optional<int> selectedIndex;
	Node* pSelectedNode = nullptr;
	struct TransformParameters{
		float rotation[3] = {0,0,0};
		float position[3] = { 0,0,0 };
		float scale[3] = { 1,1,1 };
	};
	Transform _transform;
	std::unordered_map<int, TransformParameters> transforms;
	std::unordered_map<int, Transform> _transforms;
	TransformWindow transformWindow;
	float scale = 1.0f;
};

Model::Model(Graphics& gfx, const std::string fileName, std::string folderName, std::optional<float> manualSourceUnitInMeters)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_GenBoundingBoxes |
		aiProcess_CalcTangentSpace
	);
	if (pScene == nullptr)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials, folderName));
	}
	float importScale;
	if (manualSourceUnitInMeters.has_value())
	{
		// caller told us explicitly (needed for formats with no unit metadata)
		importScale = *manualSourceUnitInMeters;
		scaleFactor = importScale * 100.0f; // keep GetScale()'s existing "relative to cm" reporting consistent
	}
	else
	{
		double rawScaleFactor = -999;
		if (pScene->mMetaData && pScene->mMetaData->Get("UnitScaleFactor", rawScaleFactor))
		{
			// FBX: UnitScaleFactor is relative to FBX's own cm baseline
			scaleFactor = static_cast<float>(rawScaleFactor);
			importScale = GameCoordinates::CentimetersToMeters(scaleFactor);

		}
		else
		{
			// No metadata available (OBJ, most glTF, etc.). glTF's spec
			// mandates meters, so assuming 1.0f is correct there; for OBJ
			// (which has no defined unit at all) this is a guess -- pass
			// manualSourceUnitInMeters explicitly for OBJ files where you
			// know the actual authored unit.
			scaleFactor = 1.0f;
			importScale = 1.0f;
			char buf[256];
			sprintf_s(buf, "[UnitScaleFactor Debug] file=%s found=%d rawScaleFactor=%f\n",
				fileName.c_str(), pScene->mMetaData ? pScene->mMetaData->Get("UnitScaleFactor", rawScaleFactor) : -1, rawScaleFactor);
			OutputDebugStringA(buf);
		}
	}
	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode);
	// --- bake the unit conversion into the root node's stored transform,
	// ONCE, here, at load time. This is the only place a scale conversion
	// happens; everything downstream (Draw, ShowWindow, any future system
	// that queries a Node's transform) sees an already-correct meters value
	// and never needs to know or care what unit the source file used.
	pRoot->transform = [&]
		{
			DirectX::XMFLOAT4X4 scaled;
			DirectX::XMStoreFloat4x4(&scaled,
				DirectX::XMMatrixScaling(importScale, importScale, importScale) *
				DirectX::XMLoadFloat4x4(&pRoot->transform)
			);
			return scaled;
		}();
	pWindow->SetScaleFactor(scaleFactor);
	// --- sanity check: does the imported (now-meters) bounding box look
	// like a plausible real-world size? Catches "forgot to apply unit
	// conversion" or "applied it twice" immediately at load time instead of
	// after an hour of "why does my model look wrong" debugging.
	{
		DirectX::XMFLOAT3 mn{ FLT_MAX, FLT_MAX, FLT_MAX };
		DirectX::XMFLOAT3 mx{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
		bool any = false;
		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			const auto& aabb = pScene->mMeshes[i]->mAABB;
			mn.x = std::min(mn.x, aabb.mMin.x); mn.y = std::min(mn.y, aabb.mMin.y); mn.z = std::min(mn.z, aabb.mMin.z);
			mx.x = std::max(mx.x, aabb.mMax.x); mx.y = std::max(mx.y, aabb.mMax.y); mx.z = std::max(mx.z, aabb.mMax.z);
			any = true;
		}
		if (any)
		{
			const float sizeX = (mx.x - mn.x) * importScale;
			const float sizeY = (mx.y - mn.y) * importScale;
			const float sizeZ = (mx.z - mn.z) * importScale;
			const float largest = std::max({ sizeX, sizeY, sizeZ });
			if (largest < GameCoordinates::SuspiciouslySmallMeters ||
				largest > GameCoordinates::SuspiciouslyLargeMeters)
			{
				std::ostringstream oss;
				oss << "[Model Import Warning] '" << fileName << "' bounding box after unit "
					<< "conversion is " << sizeX << " x " << sizeY << " x " << sizeZ
					<< " meters. This looks implausible -- check that the source unit "
					<< "detection (scaleFactor=" << scaleFactor << ", importScale=" << importScale
					<< ") is actually correct for this file.\n";
				OutputDebugStringA(oss.str().c_str());
			}
		}
	}
	_folderName = folderName;
}

float Model::GetScale() const noxnd
{
	return scaleFactor;
}

void Model::ShowWindow(const char* windowName) noexcept {
	pWindow->Show(windowName, *pRoot);
}
void Model::Draw(Graphics& gfx) const noxnd
{
	//pRoot->Draw(gfx, pWindow->GetTransform());
	if (auto node = pWindow->GetSelectedNode())
	{
		node->SetAppliedTransform(pWindow->GetTransform());
	}
	pRoot->Draw(gfx, DirectX::XMMatrixIdentity());
}
Model::~Model() noexcept
{}
std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, std::string folderName)
{
	namespace dx = DirectX;
	using Dvtx::VertexLayout;
	using namespace Bind;
	Dvtx::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Tangent)
		.Append(VertexLayout::Bitangent)
		//.Append(VertexLayout::Tangent)
		.Append(VertexLayout::Texture2D)
	));
	//auto& material = *pMaterials[mesh.mMaterialIndex];
	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
			*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i]) // -> dont forget to load this texture coords
		);
	}

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
	float shininess = 50.0f;
	std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;
	bool hasSpecularMap = false;
	bool hasNormalMap = false;
	bool hasOrmMap = false;
	bool usePBR = false;
	using namespace std::string_literals;
	const auto base = folderName;
	//if (mesh.mMaterialIndex > 0)
	//{
		//using namespace std::string_literals;
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString textFileName;
		material.GetTexture(aiTextureType_DIFFUSE, 0, &textFileName);
		bindablePtrs.push_back(Texture::Resolve(gfx, base + textFileName.C_Str()));
		const auto specResult = material.GetTexture(aiTextureType_SPECULAR, 0, &textFileName);
		{
			char buf[256];
			sprintf_s(buf, "[Specular Debug] materialIndex=%d specResult=%d filename=%s\n",
				mesh.mMaterialIndex, specResult, specResult == aiReturn_SUCCESS ? textFileName.C_Str() : "N/A");
			OutputDebugStringA(buf);
		}
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &textFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, base + textFileName.C_Str(), 1));
			hasSpecularMap = true;
			usePBR = false;
		}
		else {
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		//normal map debug

		// 1. Try standard Normals slot first
		auto normalResult = material.GetTexture(aiTextureType_NORMALS, 0, &textFileName);

		// 2. Fall back to Height slot if Normals slot is empty
		if (normalResult != aiReturn_SUCCESS)
		{
			normalResult = material.GetTexture(aiTextureType_HEIGHT, 0, &textFileName);
		}
		char buf[256];
		sprintf_s(buf, "[Normal Debug] materialIndex=%d normalResult=%d filename=%s\n",
			mesh.mMaterialIndex, normalResult, normalResult == aiReturn_SUCCESS ? textFileName.C_Str() : "N/A");
		OutputDebugStringA(buf);
		if (normalResult == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, base + textFileName.C_Str(), 2));
			hasNormalMap = true;
		}
		bindablePtrs.push_back(Bind::Sampler::Resolve(gfx));
		//auto metallicResult = material.GetTexture(aiTextureType_METALNESS, 0, &textFileName);
		//sprintf_s(buf, "[Metallic Debug] materialIndex=%d metallicResult=%d filename=%s\n",
		//	mesh.mMaterialIndex, metallicResult, metallicResult == aiReturn_SUCCESS ? textFileName.C_Str() : "N/A");
		//OutputDebugStringA(buf);
		//if (metallicResult == aiReturn_SUCCESS)
		//{
		//	bindablePtrs.push_back(Texture::Resolve(gfx, base + textFileName.C_Str(), 3));
		//	hasMetallicMap = true;
		//	usePBR = true;
		//}
		//auto aoResult = material.GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textFileName);
		//sprintf_s(buf, "[AO Debug] materialIndex=%d AoResult=%d filename=%s\n",
		//	mesh.mMaterialIndex, aoResult, aoResult == aiReturn_SUCCESS ? textFileName.C_Str() : "N/A");
		//OutputDebugStringA(buf);
		//if (aoResult == aiReturn_SUCCESS)
		//{
		//	bindablePtrs.push_back(Texture::Resolve(gfx, base + textFileName.C_Str(), 4));
		//	hasAoMap = true;
		//	usePBR = true;
		//}
		//else {
		//	// check if we have a map with name AO.png
		//	bindablePtrs.push_back(Texture::Resolve(gfx, base + "\\AO.png", 4));
		//	hasAoMap = true;
		//	usePBR = true;
		//}
		auto ormResult = material.GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &textFileName);
		sprintf_s(buf, "[Roughness Debug] materialIndex=%d RoughnessResult=%d filename=%s\n",
			mesh.mMaterialIndex, ormResult, ormResult == aiReturn_SUCCESS ? textFileName.C_Str() : "N/A");
		OutputDebugStringA(buf);
		if (ormResult == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, base + textFileName.C_Str(), 3));
			hasOrmMap = true;
			usePBR = true;
		}
	//}
	auto meshTag = base + "%" + mesh.mName.C_Str();
	bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

	bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

	auto pvs = VertexShader::Resolve(gfx, "PhongShadingVS_NRML.cso");// : "PhongShadingVS.cso");
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

	// shader mapping trick 
	// if(!hasMetallic && !hasAO && !hasRoughness) => phong shading with cnostant specular factors
	// if(!hasMetallic || !hasAo || !hasRoughness) => PBR
	// if(hasSpeculatMap) => definetly phong shading

	if (hasSpecularMap)
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongShadingNS_PS.cso"));
	}
	if (usePBR)
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PBR_Metal_Roughness.cso"));
		//sprintf_s(buf, "[Shader Debug] loading material %s for model %s\n","PBR_Metal_Roughness",mesh.mName);
		//OutputDebugStringA(buf);
	}
	/*if (hasSpecularMap && !hasNormalMap)
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongShadingSpechPS.cso"));
	}
	else if (hasSpecularMap && hasNormalMap)
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongShadingNS_PS.cso"));
	}
	else
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongShadingPS.cso"));
	}*/


	

	struct PSMaterialConstant
	{
		//DirectX::XMFLOAT3 color = { 0.6f,0.6f,0.8f };
		float specularIntensity = 1.0f;
		float specularPower;
		BOOL useSpecularMap = FALSE; // BOOL = int, not C++ bool -- guarantees 4 bytes, matches HLSL's int
		BOOL useNormalMap = FALSE;
		float metallicFactor = 0;
		float roughnessFactor = 0.4;
		float ambientFactor = 0;
		BOOL useORMMap = FALSE;
	} pmc;
	pmc.specularPower = shininess;
	pmc.useNormalMap = hasNormalMap ? TRUE : FALSE;
	pmc.useSpecularMap = hasSpecularMap ? TRUE : FALSE;
	pmc.useORMMap = hasOrmMap ? TRUE : FALSE;
	bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}
std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node) noexcept
{
	namespace dx = DirectX;
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, std::move(curMeshPtrs), transform, std::string(node.mName.C_Str()));
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
	}

	return pNode;
}