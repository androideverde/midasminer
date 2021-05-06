#include <CInputHandler.h>

#include <CApp.h>

CInputHandler::CInputHandler(std::function<void()> exitCallback)
	: mExitCallback(exitCallback)
{
}

void CInputHandler::OnEvent(SDL_Event *event)
{
	switch (event->type) {
		case SDL_QUIT:
			OnExit();
			break;
		case SDL_MOUSEMOTION:
			OnMouseMove(event->motion.x, event->motion.y,
						event->motion.xrel, event->motion.yrel,
						(event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
						(event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
						(event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					OnLButtonDown(event->button.x, event->button.y);
					break;
				case SDL_BUTTON_RIGHT:
					OnRButtonDown(event->button.x, event->button.y);
					break;
				case SDL_BUTTON_MIDDLE:
					OnMButtonDown(event->button.x, event->button.y);
					break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			OnButtonUp(event->button.x, event->button.y);
			break;
		default:
			break;
	}
}

void CInputHandler::OnExit()
{
	mExitCallback();
}

void CInputHandler::OnMouseMove(int x, int y, int delta_x, int delta_y, bool l_button, bool r_button, bool m_button)
{
//	if (mDragging)
//	{
//		SBoardCoords currentTileCoords = mBoard.GetBoardTileCoords(x, y);
//		if (mTileClickedCoords.row != -100 && currentTileCoords.row != -100)
//		{
//			if (!(mTileClickedCoords == currentTileCoords))
//			{
//				mBoard.OnDrag(mTileClickedCoords, currentTileCoords);
//				mDragging = false;
//			}
//		}
//	}
}

void CInputHandler::OnLButtonDown(int x, int y)
{
//	mTileClickedCoords = mBoard.GetBoardTileCoords(x, y);
//	if (mTileClickedCoords.row != -100)
//	{
//		mDragging = true;
//	}
}

void CInputHandler::OnRButtonDown(int x, int y)
{
}

void CInputHandler::OnMButtonDown(int x, int y)
{
}

void CInputHandler::OnButtonUp(int x, int y)
{
//	if (mDragging)
//	{
//		SBoardCoords currentTileCoords = mBoard.GetBoardTileCoords(x, y);
//		if (mTileClickedCoords.row != -100 && currentTileCoords.row != -100)
//		{
//			if (mTileClickedCoords == currentTileCoords)
//			{
//				mBoard.OnClick(currentTileCoords);
//				mDragging = false;
//			}
//		}
//	}
}
