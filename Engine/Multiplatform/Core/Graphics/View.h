#pragma once

#include "StdInterface/Updatable.h"
#include "Graphics/GraphicalProgram.h"
#include "Math/Geometry/Transform.h"
#include <unordered_map>

namespace Neptune
{
	class Renderer;

	class View : public Updatable
	{
	public:
		View()                      = default;
		virtual ~View()              {}
		View(const View&)            = delete;
		View& operator=(const View&) = delete;

		bool init()             override;
		bool update()           override;
		void terminate()        override;

		Transform& getTransform() { return m_transform; } 
		Renderer& getRenderer()   { return *m_renderer; }

		// Implemented to fix a warning stating this class should be 16-bit aligned
		// Further investigation would be appreciated though
		void* operator new(std::size_t count);
		void  operator delete(void* ptr);

	protected:
		Transform      m_transform;
		Renderer*      m_renderer;
	};
}