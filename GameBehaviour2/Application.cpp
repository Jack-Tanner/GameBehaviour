#include "Application.h"
#include "BaseInputManager.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <TextureManager.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "Helpers.h"
#include "Debug.h"
#include "World.h"

#include "SpriteEntity.h"



Application::Application() : m_bExit ( true ), m_bShutdownRequested( false )
{
}

Application::~Application()
{
}

bool Application::Initialise()
{
	SDL_Init( SDL_INIT_EVERYTHING );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8 );

	if ( !m_Window.Create( "Game Behaviour 2", 400, 100, 1280, 720 ) )
		return false;

	m_Context = SDL_GL_CreateContext( m_Window.GetWindow() );	
	
	if ( !m_Renderer.Initialise() )
		return false;

	m_bExit = false;
	return true;
}

void Application::Run()
{
	BaseInputManager* pInputManager = BaseInputManager::GetInstance();
	GASSERT( pInputManager );	

	BaseShader* pBasicShader = m_Renderer.GetShaderManager().CreateShaderFromFile( "Basic", "BasicVertexColour_Vert.glsl", "BasicVertexColour_Frag.glsl" );

	// Try to scale 1 unit to 50pixels
	float xRatio = 100.0f;
	float yRatio = 100.0f;
	float x = ( 1280 * 0.5f );
	float y = ( 720 * 0.5f );

	glm::mat4 projMatrix = glm::orthoLH( -x, x, -y, y, 1.0f, 100000000.0f );
	glm::vec3 eyePos( 0.0f, 0.0f, 0.0f);
	glm::vec3 lookAt( 0.0f, 0.0f, 1.0f );
	glm::vec3 Up(0.0f, 1.0f, 0.0f);

	glm::mat4 viewMatrix = glm::lookAtLH( eyePos, lookAt, Up );

	m_Renderer.GetShaderManager().UseShader( pBasicShader );
	pBasicShader->SetViewMatrix( viewMatrix );
	pBasicShader->SetProjectionMatrix( projMatrix );

	BaseShader* pTexturedShader = m_Renderer.GetShaderManager().CreateShaderFromFile( "BasicTextured", "BasicTextured_Vert.glsl", "BasicTextured_Frag.glsl" );
	m_Renderer.GetShaderManager().UseShader( pTexturedShader );
	pTexturedShader->SetViewMatrix( viewMatrix );
	pTexturedShader->SetProjectionMatrix( projMatrix );
	GLuint uTexLocation = pTexturedShader->GetUniformLocation( "texColour" );
	pTexturedShader->SetUniform( uTexLocation, 0 );

	BaseShader* pAnimatedTexturedShader = m_Renderer.GetShaderManager().CreateShaderFromFile("AnimatedTextured", "AnimatedTextured_Vert.glsl", "BasicTextured_Frag.glsl");
	m_Renderer.GetShaderManager().UseShader(pAnimatedTexturedShader);
	pAnimatedTexturedShader->SetViewMatrix( viewMatrix );
	pAnimatedTexturedShader->SetProjectionMatrix(projMatrix);
	uTexLocation = pAnimatedTexturedShader->GetUniformLocation("texColour");
	pAnimatedTexturedShader->SetUniform(uTexLocation, 0);


	GLFont* m_pFont = m_Renderer.GetFontManager().AddFont( "Assets/Fonts/arial.ttf", 36 );

	m_GameStateManager.SetGameState( eGameState::eBoot );

	float fFrameDelta;
	Uint64 uFrameTime = SDL_GetPerformanceCounter();

	SDL_Event windowEvent;
	while ( !m_bExit )
	{
		fFrameDelta = (float)( ( SDL_GetPerformanceCounter() - uFrameTime )  ) / SDL_GetPerformanceFrequency();
		uFrameTime = SDL_GetPerformanceCounter();

		while ( SDL_PollEvent( &windowEvent ) )
		{
			if ( windowEvent.type == SDL_QUIT )
			{
				m_bShutdownRequested = true;
			}
		}

		// Swaps input states - must be first.
		pInputManager->Update();

#if _DEBUG
		if ( pInputManager->IsKeyPressed( SDL_SCANCODE_ESCAPE ) )
		{
			m_bShutdownRequested = true;
		}
#endif

		m_GameStateManager.Update( fFrameDelta );

		if ( pInputManager->IsKeyPressed( SDL_SCANCODE_K ) )
		{
			m_GameStateManager.SetGameState( eGameState::eGame );
		}

		static bool bPhysics = true;
		if (pInputManager->IsKeyPressed(SDL_SCANCODE_P))
			bPhysics = !bPhysics;

		if(bPhysics)
			m_PhysicsManager.Update( fFrameDelta );

		m_Renderer.Render();

		std::stringstream ss;
		ss << roundf( ( fFrameDelta * 1000.0f ) * 100.0f ) / 100.0f << "ms";
		//ss << pEntity->GetTransform().m_Velocity.y;
		std::string strFPS = ss.str();
		m_Renderer.GetFontManager().PrintString( m_pFont, strFPS, -640.0f, 360.0f - m_pFont->GetHeight(), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

		m_Window.SwapBuffer();

		if ( m_bShutdownRequested )
		{
			m_bExit = true;
		}
	}
}

void Application::Shutdown()
{
	m_GameStateManager.SetGameState( eGameState::eNone );
	GL_CHECK();
	m_Renderer.Shutdown();
	GL_CHECK();

	SDL_GL_DeleteContext( m_Context );
	SDL_Quit();
}
