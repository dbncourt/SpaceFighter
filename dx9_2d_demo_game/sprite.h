//-----------------------------------------------------------------------------
//           Name: sprite.h
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: Declaration of Sprite class for gathering together all 
//                 properties and methods related to managing D3DXSprite based 
//                 game objects.
//-----------------------------------------------------------------------------

#include <d3d9.h>
#include <d3dx9.h>


#ifndef CSPRITE_H_INCLUDED

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
const int TOTAL_ANIMATIONS =  5; // Max number of animation states per sprite
const int MAX_SIZE         = 25;

class CSprite
{

public:

	enum AnimeEndOption
	{
		LOOP_ANIMATION = 200,
		GOTO_NEXT_ANIMATION,
		MAINTAIN_LAST_FRAME,
		GO_INACTIVE,
		DESTROY_SPRITE
	};

	CSprite();
	~CSprite();
	void zeroSpriteValues();
	void releaseMemory();
	void incFrame( bool bUseModifier = true );
	void decFrame( bool bUseModifier = true );
	void loadAnimation( int nAnimeNumber, int nStartFrame, int nEndFrame, 
		                AnimeEndOption nOption, int nNextAnimation = NULL );
	void loadAnimationString( int nAnimeNumber, char *chString, 
		                      AnimeEndOption nOption, int nNextAnimation = NULL );
	HRESULT drawSprite( LPDIRECT3DDEVICE9 pDevice );
	
	LPDIRECT3DTEXTURE9 m_pSpriteTexture;
	LPD3DXSPRITE       m_pD3DXSprite;
	int		m_nWidth;
	int		m_nHeight;

	int     m_nID;
	int     m_nState;
	char	m_chType[MAX_SIZE];
	char	m_chName[MAX_SIZE];
	char	m_chSpriteTextureName[MAX_SIZE];
	float	m_fPosition_x;
	float	m_fPosition_y;
	float	m_fVelocity_x;
	float	m_fVelocity_y;
	bool	m_bVisible;
	bool	m_bCollide;
	bool    m_bAutoAnimate;
	bool    m_bActive;
	bool    m_bScripting;
	bool    m_bModifyCollision;
	bool    m_bSingleFrame;
	bool    m_bDestroy;
	int     m_nFrameRateModifier;
	int		m_nFrameWidth;
	int		m_nFrameHeight;
	int		m_nFramesAcross;
	int     m_nFrameOffset_x;
	int     m_nFrameOffset_y;
	int     m_nWidthScaling;
	int     m_nHeightScaling;
	int 	m_nCurrentAnimation;
	int		m_nCurrentFrame;
	int		m_nCurrentScript;
	int     m_nCollisionTop;
	int     m_nCollisionBottom;
	int     m_nCollisionLeft;
	int     m_nCollisionRight;

private:
	
	int    *m_nAnimations[TOTAL_ANIMATIONS];
	int     m_nFrameCount[TOTAL_ANIMATIONS];
	int     m_nFrameSkipCount;
	
};

#define CSPRITE_H_INCLUDED
#endif /* CSPRITE_H_INCLUDED */



//-----------------------------------------------------------------------------
// Notes on defines and variables:
//-----------------------------------------------------------------------------
/*	

	The following defines are used to terminate an animation sequence.
	The codes tell the game what should be done after the last animation 
	frame has been displayed.

	LOOP_ANIMATION       - Loop current animation until further notified.
	GOTO_NEXT_ANIMATION  - Change animation state to the sate number stored next in the array.
	MAINTAIN_LAST_FRAME  - When the animation sequence is finished, keep blitting the last frame of current animation.
	GO_INACTIVE          - When the animation sequence is finished, the sprite should go into a standby mode and wait.
	DESTROY_SPRITE       - When the animation sequence is finished, destroy the sprite by removing it from the linked list.

	Notes on Member Variables:

	m_chType              - Identifies the sprite's type. (i.e. ship, enemy, bomb)
	m_chName              - Unique identifier for the sprite. (i.e. player1, player2, levelBoss)
	m_nState              - Identifies the sprite's current state. (i.e. normal, powered-up, exploding)
	m_nID                 - Simple numerical identifier for use by the programmer
	m_dPosition_x         - Sprite's x coordinate position on the screen.
	m_dPosition_y         - Sprite's y coordinate position on the screen.
	m_dVelocity_x         - Sprite's velocity along the x axis.
	m_dVelocity_y         - Sprite's velocity along the y axis. 
	m_bVisible            - Is the sprite visible or not?
	m_bDestroy            - Should the sprite be removed from the linked list?
	m_bCollide            - Should collision detection be performed on this sprite?
	m_bAutoAnimate        - Should the sprite animate itself or will the programmer do it manually?
	m_bActive             - Is the sprite playing an active role in the game or is it in stanby?
	m_bScripting          - Is this sprite currently under script control?
	m_bModifyCollision    - Will the size or shape of the collision area be modified before collision detection is performed?
	m_bSingleFrame        - Does this sprite consist of a single frame and has no animations?
	m_nFrameRateModifier  - Either a negative or positive number whic is used to slow down or speed up the frame rate for animations.
	m_nFrameWidth         - The width of each frame that makes up the sprite's animation.
	m_nFrameHeight        - The height of each frame that makes up the sprite's animation.
	m_nFramesAcross       - The number of frames per row in the bitmap.
	m_nFrameOffset_x      - If the first pixel of the sprite's first frame is not located in 
	                        the upper left corner of the bitmap, you must give the first pixel's 
						    actual x coordinate position or the frames will not be extracted 
						    correctly.
	m_nFrameOffset_y      - Same as above, but holds the y coordinate position.
	m_nWidthScaling       - The number of pixels to increase or decrease the sprite's width by when blitting
	m_nHeightScaling      - The number of pixels to increase or decrease the sprite's height by when blitting
	m_nCollisionTop       - The number of pixels to increase or decrease the sprite's top for collison detection
	m_nCollisionBottom    - The number of pixels to increase or decrease the sprite's bottom for collison detection
	m_mCollisionLeft      - The number of pixels to increase or decrease the sprite's left side for collison detection
	m_mCollisionRight     - The number of pixels to increase or decrease the sprite's right side for collison detection
	m_nCurrentAnimation   - The index number of the animation sequence that the sprite
						    is currently using.
	m_nCurrentFrame       - The frame within the animation sequence that the sprite is 
						    currently using.
	m_nAnimations[]       - An array of pointers to int arrays where the frame numbers that 
	                        make up an animation sequence will be stored.
	m_nFrameCount[]       - Keeps track of the number frames per animationn sequence.

*/
 
