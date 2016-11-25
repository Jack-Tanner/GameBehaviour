#include "Renderable.h"

#include "Renderer.h"

Renderable::Renderable() : m_bInitialised( false ), m_WorldMatrix( 1.0f )
{
	Renderer::GetInstance()->AddRenderable( this );
	m_pRenderer = Renderer::GetInstance();
}

Renderable::~Renderable()
{
	if ( m_bInitialised )
	{
		Destroy();
	}
}

void Renderable::Destroy()
{
	if ( m_pRenderer )
	{
		m_pRenderer->RemoveRenderable( this );
	}

	m_bInitialised = false;
}
