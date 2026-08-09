#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include "ConditionalNoexcept.h"
#include <memory>

namespace Bind
{
	class Bindable;
	class IndexBuffer;
}

//class Bindable;

class Drawable
{
	/*template<class T>
	friend class DrawableBase;*/
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noxnd;
	//virtual void Update(float dt) noexcept {}
	virtual ~Drawable() = default;
protected:
	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto p = dynamic_cast<T*>(pb.get()))
			{
				return p;
			}
		}
		return nullptr;
	}
	//void AddBind(std::unique_ptr<Bind::Bindable> bind) noxnd;
	//void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noxnd;
//private:
	//virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept = 0;
	void addBind(std::shared_ptr<Bind::Bindable> bind) noxnd;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	//std::vector<std::unique_ptr<Bind::Bindable>> binds;
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
};
