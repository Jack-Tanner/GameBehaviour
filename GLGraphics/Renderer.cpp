#include "Renderer.h"
#include "Debug.h"

#include "GLBuffer.h"

Renderer* Renderer::m_pInstance = nullptr;

Renderer::Renderer()
{
	GASSERT( m_pInstance == nullptr );
	m_pInstance = this;
}

Renderer::~Renderer()
{
	m_pInstance = nullptr;
}

bool Renderer::Initialise()
{
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable( GL_MULTISAMPLE );
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	
	glBindVertexArray( 0 );

	glGenVertexArrays( 1, &m_debugVAO );

	glLineWidth( 2.0f );

	m_FontManager.Initialise();
	
	return true;
}

void Renderer::Render()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	for ( auto& renderable : m_Renderables )
	{
		renderable->Render();
	}


	// SLOW, HACKY DEBUG DRAWING.
	glBindVertexArray( m_debugVAO );
	BaseShader* pDebugShader = m_ShaderManager.GetShader( "Basic" );
	if ( pDebugShader )
	{
		m_ShaderManager.UseShader( pDebugShader );
		pDebugShader->SetWorldMatrix( glm::mat4( 1.0f ) );

		for ( auto& debugPrim : m_DebugPrimitives )
		{
			if ( debugPrim )
			{
				if ( debugPrim->m_eType == eLine )
				{
					DebugLine* pLine = ( DebugLine* ) debugPrim;
					if ( pLine )
					{
						GL_CHECK();
						glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

						GLfloat verts[] = {
							pLine->m_vStart.x, pLine->m_vStart.y, 1.0f, pLine->m_vColour.r, pLine->m_vColour.g, pLine->m_vColour.b, pLine->m_vColour.a,
							pLine->m_vEnd.x, pLine->m_vEnd.y, 1.0f, pLine->m_vColour.r, pLine->m_vColour.g, pLine->m_vColour.b, pLine->m_vColour.a
						};

						GLBuffer vbo;
						vbo.Initialise( BufferType::eVertexBuffer, GL_STATIC_DRAW, &verts, sizeof( verts ) );
						vbo.BindBuffer();

						const AttributeDescriptor* pPositionAttrib = pDebugShader->GetAttribute( ShaderAttribute::ePosition );
						const AttributeDescriptor* pVertexColourAttrib = pDebugShader->GetAttribute( ShaderAttribute::eVertexColour );

						glEnableVertexAttribArray( pPositionAttrib->m_Location );
						glVertexAttribPointer( pPositionAttrib->m_Location, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, 0 );
						glEnableVertexAttribArray( pVertexColourAttrib->m_Location );
						glVertexAttribPointer( pVertexColourAttrib->m_Location, 4, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, ( void* ) ( 3 * sizeof( GLfloat ) ) );

						glDrawArrays( GL_LINES, 0, 2 );
						glBindBuffer( GL_ARRAY_BUFFER, 0 );
						vbo.Destroy();
						GL_CHECK();
					}
				}
				else if( debugPrim->m_eType == eQuad )
				{
					DebugQuad* pQuad = ( DebugQuad* ) debugPrim;
					GL_CHECK();
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

					GLfloat verts[] = {
						pQuad->m_vCorners[0].x, pQuad->m_vCorners[0].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,
						pQuad->m_vCorners[1].x, pQuad->m_vCorners[1].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,

						pQuad->m_vCorners[1].x, pQuad->m_vCorners[1].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,
						pQuad->m_vCorners[2].x, pQuad->m_vCorners[2].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,

						pQuad->m_vCorners[2].x, pQuad->m_vCorners[2].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,
						pQuad->m_vCorners[3].x, pQuad->m_vCorners[3].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,

						pQuad->m_vCorners[3].x, pQuad->m_vCorners[3].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a,
						pQuad->m_vCorners[0].x, pQuad->m_vCorners[0].y, 1.0f, pQuad->m_vColour.r, pQuad->m_vColour.g, pQuad->m_vColour.b, pQuad->m_vColour.a
					};

					GLBuffer vbo;
					vbo.Initialise( BufferType::eVertexBuffer, GL_STATIC_DRAW, &verts, sizeof( verts ) );
					vbo.BindBuffer();

					const AttributeDescriptor* pPositionAttrib = pDebugShader->GetAttribute( ShaderAttribute::ePosition );
					const AttributeDescriptor* pVertexColourAttrib = pDebugShader->GetAttribute( ShaderAttribute::eVertexColour );

					glEnableVertexAttribArray( pPositionAttrib->m_Location );
					glVertexAttribPointer( pPositionAttrib->m_Location, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, 0 );
					glEnableVertexAttribArray( pVertexColourAttrib->m_Location );
					glVertexAttribPointer( pVertexColourAttrib->m_Location, 4, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, ( void* ) ( 3 * sizeof( GLfloat ) ) );

					glDrawArrays( GL_LINES, 0, 8 );
					glBindBuffer( GL_ARRAY_BUFFER, 0 );
					vbo.Destroy();
					GL_CHECK();
				}

				delete debugPrim;
			}
		}

		m_DebugPrimitives.clear();
	}
	glBindVertexArray( 0 );
}

void Renderer::Shutdown()
{
	// Renderable->Destroy() removes itself from the renderable list which invalidates iterator,
	// TODO: Look at this.
	while( m_Renderables.size() > 0 )
	{
		if ( m_Renderables[0] == nullptr )
			break;

		m_Renderables[0]->Destroy();
	}

	m_FontManager.Shutdown();

	m_Renderables.clear();
}

void Renderer::AddRenderable( Renderable * pRenderable )
{
	if ( pRenderable )
		m_Renderables.push_back( pRenderable );
}

void Renderer::RemoveRenderable( Renderable * pRenderable )
{
	for ( auto it = m_Renderables.begin(); it != m_Renderables.end(); it++ )
	{		
		if ( ( *it ) == pRenderable )
		{
			m_Renderables.erase( it );
			return;
		}
	}
}

void Renderer::DrawDebugLine( const glm::vec2& vStart, const glm::vec2& vEnd, glm::vec4 vColour )
{
	DebugLine* pLine = new DebugLine();
	pLine->m_eType = DebugPrimitiveType::eLine;
	pLine->m_vStart = vStart;
	pLine->m_vEnd = vEnd;
	pLine->m_vColour = vColour;

	m_DebugPrimitives.push_back( pLine );
}

void Renderer::DrawDebugQuad( glm::vec2 * vCorners, glm::vec4 vColour )
{
	DebugQuad* pQuad = new DebugQuad();
	pQuad->m_eType = DebugPrimitiveType::eQuad;
	pQuad->m_vCorners[0] = (*vCorners++);
	pQuad->m_vCorners[1] = ( *vCorners++ );
	pQuad->m_vCorners[2] = ( *vCorners++ );
	pQuad->m_vCorners[3] = ( *vCorners );
	pQuad->m_vColour = vColour;

	m_DebugPrimitives.push_back( pQuad );
}

Renderer * Renderer::GetInstance()
{
	return m_pInstance;
}
