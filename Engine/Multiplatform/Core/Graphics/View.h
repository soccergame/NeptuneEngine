#pragma once

#include "StdInterface/Updatable.h"
#include "Math/Geometry/Transform.h"

namespace Neptune
{
	class Renderer;
	class Camera;
	class AmbientLight;

	class View : public Updatable
	{
	public:
		View(): m_camera(nullptr),
				m_ambientLight(nullptr),
			    m_renderer(nullptr)  {}
		virtual ~View()              {}
		View(const View&)            = delete;
		View& operator=(const View&) = delete;

		bool init()      override;
		bool update()    override;
		void terminate() override;

		Transform& getTransform()    { return m_transform; } 
		Renderer&  getRenderer()     { return *m_renderer; }

		void bindToCamera(Camera* c) { m_camera = c;       }
		void unbindFromCamera()      { m_camera = nullptr; }

		void bindToAmbientLight(AmbientLight* _a) { m_ambientLight = _a;      }
		void unbindFromAmbientLight()             { m_ambientLight = nullptr; }

		// Implemented to fix a warning stating this class should be 16-bit aligned
		// Further investigation would be appreciated though
		void* operator new(std::size_t count);
		void  operator delete(void* ptr);

	protected:
		Transform      m_transform;
		Renderer*      m_renderer;
		Camera*        m_camera;
		AmbientLight*  m_ambientLight;
	};
}