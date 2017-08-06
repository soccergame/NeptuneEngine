#include "Graphics/View.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/Texture.h"

#include "Debug/NeptuneDebug.h"
#include "System/Hashing/FastHashFunctions.h" //! the hash function must be used by the unordered_map directly

using namespace Neptune;

bool View::init()
{
	return m_renderer->init();
}

bool View::update()
{
	bool status = true;
	
	if(m_texture != nullptr)
	{
		NEP_ASSERT(m_texture->isInitialized());
		status = status && m_texture->update();
	}

	if ( m_camera != nullptr )
	{
		// Ouch dirtyyyyyyyy! mvp sent in mv uniform because the camera binding is messy! FIXME PLEASE
		Mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * m_transform.getMatrix();
		
		updateUniform(NEP_UNIVNAME_MV_MATRIX,(void*)mvp.getPtr());
	}
	else 
	{
		updateUniform(NEP_UNIVNAME_MV_MATRIX,(void*)m_transform.getDataPtr());
	}

	status = status && m_renderer->update();
	return status;
}

bool View::updateUniform(const char* _pgmName, const char* _uniformName, void* _value)
{
	const char* key = (const char*) Fnv1a32((u8*) _pgmName, strlen(_pgmName)); // hash should be done automatically by the container
	auto        it  = m_programMap.find(key);
	
	NEP_ASSERT(it != m_programMap.end()); //  Error: Program doesn't exist

	return m_renderer->updateUniform(it->second, _uniformName, _value);
}

bool View::updateUniform(const char* _uniformName, void* _value)
{
	bool status = false;
	
	for (const auto& it : m_programMap)
	{
		Renderer::ProgramID id = it.second;

		status = status || m_renderer->updateUniform(id, _uniformName, _value);
	}
	
	return status;
}

void View::addGraphicsProgram(const char* _pgmName, GraphicsProgram* _pgm)
{
	NEP_ASSERT(m_programMap.find(_pgmName) == m_programMap.end()); // Error: two programs have the same name
	
	Renderer::ProgramID id       = m_renderer->addProgram( _pgm );
	m_programMap[(const char*) Fnv1a32((u8*) _pgmName, strlen(_pgmName))] = id; // call to hash must be removed. The map should do it itself
}

void View::terminate()
{
	m_renderer->terminate();
}

void* View::operator new(size_t count)
{
	void* ptr = _aligned_malloc(count,16);
	return ptr;
}

void View::operator delete(void* ptr)
{
	_aligned_free(ptr);
}