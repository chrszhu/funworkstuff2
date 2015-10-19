#include <iostream>
using namespace std;

#include <string>
#include "Game.h"
#include "globals.h"
#include "Pit.h"
#include "Player.h"
///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nSnakes)
{
	if (nSnakes < 0)
	{
		cout << "***** Cannot create Game with negative number of snakes!" << endl;
		exit(1);
	}
	if (nSnakes > MAXSNAKES)
	{
		cout << "***** Cannot create Game with " << nSnakes
			<< " snakes; only " << MAXSNAKES << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nSnakes > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the snakes!" << endl;
		exit(1);
	}

	// Create pit
	m_pit = new Pit(rows, cols);

	// Add player
	int rPlayer = 1 + rand() % rows;
	int cPlayer = 1 + rand() % cols;
	m_pit->addPlayer(rPlayer, cPlayer);

	// Populate with snakes
	while (nSnakes > 0)
	{
		int r = 1 + rand() % rows;
		int c = 1 + rand() % cols;
		// Don't put a snake where the player is
		if (r == rPlayer  &&  c == cPlayer)
			continue;
		m_pit->addSnake(r, c);
		nSnakes--;
	}
}

Game::~Game()
{
	delete m_pit;
}

void Game::play()
{
	bool pause = false;
	Player* p = m_pit->player();
	if (p == nullptr)
	{
		m_pit->display("");
		return;
	}
	string msg = "";
	while (!m_pit->player()->isDead() && m_pit->snakeCount() > 0)
	{
		m_pit->display(msg);
		msg = "";
		cout << endl;
		cout << "Move (u/d/l/r//h/q): ";
		string action;
		getline(cin, action);
		if (action.size() == 0)
			p->stand();
		else
		{
			switch (action[0])
			{
			default:   // if bad move, nobody moves
				cout << '\a' << endl;  // beep
				continue;
			case 'q':
				return;
			case 'u':
			case 'd':
			case 'l':
			case 'r':
				p->move(decodeDirection(action[0]));
				break;
			case 'h':
				m_pit->history().display();
				cout << "Press enter to continue.";
				cin.ignore(10000, '\n');
				pause = true;
				break;
			}
		}
		if (!pause)
		{
			m_pit->moveSnakes();
		}
		else
		{
			pause = false;
		}
	}
	m_pit->display(msg);
}