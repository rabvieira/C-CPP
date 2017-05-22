//*****************************************************************************
// Name      : songs.h
// Date	     : April 7, 2015
// Class     : CEC322, Microprocessor Systems Lab
// Authors   : Renan Vieira & Jared Johnson
// Descript. : defines two array of frequencies in order to play two different
//			   musics using PWM peripheral
//*****************************************************************************

/* constants */
#define RT	000.0000000000 //rest

//Ode to Joy: Beethoven Symphony No 9 (tempo: 90)
#define C4	261.6255653006
#define D4	293.6647679174
#define E4	329.6275569129
#define F4	349.2282314330
#define G4	391.9954359817

//Hedwig's Theme - Harry Potter (tempo: 58)
#define A4	440.0000000000
#define B4	493.8833012561
#define A5	880.0000000000
#define B5	987.7666025122
#define D5s	554.3652619537
#define E5	659.2551138257
#define F5	698.4564628660 
#define F5s	739.9888454233
#define G5	783.9908719635 
#define G5s	830.6093951599 
#define C6	1046.5022612024
#define C6s	1108.7305239075
#define D6	1174.6590716696

//Ode to Joy: Beethoven Symphony No 9
const float beethoven9[] = {E4, E4, F4, G4, G4, F4, E4, D4, C4, C4, D4, E4, 
                            E4, D4, D4, RT,
                            E4, E4, F4, G4, G4, F4, E4, D4, C4, C4, D4, E4,
                            D4, C4, C4, RT,
                            D4, D4, E4, C4, D4, F4, E4, C4, D4, F4, E4, D4,
                            C4, D4, G4, RT,
                            E4, E4, F4, G4, G4, F4, E4, D4, C4, C4, D4, E4,
                            D4, C4, C4, RT, -1};

//Hedwig's Theme - Harry Potter	
const float harryPotter[]= {B4, RT, E5, RT, RT, G5, F5s, RT, E5, RT, RT, RT, B5, 
                            RT, A5, RT, RT, RT, RT, RT, F5s, RT, RT, RT, RT, RT,
                            E5, RT, RT, G5, F5, RT, D5s, RT, RT, RT, F5, RT,
                            B4, RT, RT, RT, RT, B4, RT,
                            E5, RT, RT, G5, F5s, RT, E5, RT, RT, RT, B5, RT, D6,
                            RT, RT, RT, C6s, RT, C6, RT, RT, RT, G5s, RT,
                            C6, RT, RT, B5, G5s, RT, A4, RT, RT, RT, G5, RT, E5, 
                            RT, RT, RT, RT, RT, RT, -1};
							