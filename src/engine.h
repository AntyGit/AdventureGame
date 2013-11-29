#ifndef ENGINE_H
#define ENGINE_H
#include "file_handler.h"
#include "message.h"
#include "player.h"
#include "world.h"

class Engine
{
public:
	Engine();
	~Engine();
	//enum eEngineStatus{};
	EngineMessage* setOutput() const
	{
		return m_enginemessage;
	}
	
	void 			setInput(InputMessage* input_message);
	bool 			update();
private:
	bool			m_running;
	EngineMessage*	m_enginemessage;	//temporär lagring
	InputMessage*	m_inputmessage;		//temporär lagring
	bool 			m_godmode;
	FileHandler 	m_filehandler;
	World 			m_world;
	Player 			m_player;
};

#endif