#include "headers.h"
#include "fonttype.h"

void echiquier(SDL_Renderer* renderer)
{
    int ligne;
	int colonne;
	int x = 0;
    SDL_Rect rect, darea;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);

    for(ligne = 0; ligne < 8; ligne++)
    {
        colonne = ligne%2;
        x = colonne;
        for(colonne = 0; colonne < 4+(ligne%2); colonne++)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

            rect.w = darea.w/8;
            rect.h = darea.h/8;
            rect.x = x * rect.w;
            rect.y = ligne * rect.h;
            x = x + 2;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

bool collision(SDL_Rect &A, SDL_Rect &B)
{
	//Les côtes des rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	
	//Calcul les côtes du rectangles A
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;
	
	//Calcul les côtes du rectangles B
	leftB = B.x;
	rightB = B.x + B.w;	
	topB = B.y;
	bottomB = B.y + B.h;
	
	//Test de collision
	if (bottomA <= topB)
	{
		return false;
	}
	
	if (topA >= bottomB)
	{
		return false;
	}
	
	if (leftA >= rightB)
	{
		return false;
	}
	
	if (rightA <= leftB)
	{
		return false;
	}
	
	//Si conditions collision detectée
	return true;
}

int main(int argc, char* args[])
{	
	//Initialisation de SDL2
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024); //Fréquence, Format, Nombre channel, Taille d'échantillon
	
	//Initialisation 
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	TTF_Font* font = TTF_OpenFont("font/RustedOrlando.ttf", 500);

	window = SDL_CreateWindow("Echec", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 860, 740, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	
	if (window == NULL) 
	{
        printf("Erreur de chargement de la fenetre : %s", SDL_GetError());
        return 1;
    }
    
	Mix_Music* music = NULL;
	//music = Mix_LoadMUS("musics/theme.mp3");
	Mix_PlayMusic(music, -1);
	
	Mix_Chunk* select = NULL;
	//select = Mix_LoadWAV( "effects/select.wav" );
	
	Mix_Chunk* cursomove = NULL;
	//cursomove = Mix_LoadWAV( "effects/cursomove.wav" );

    //Déclaration des variables
    int continuer = 1;
    int difficulter = 1;
    int bleuvsia = 1;
    int choix = 1;
    int niveau1_bleuvsrouge = 1;
	int niveau2_bleuvsrouge = 1;
	int joueur1 = 1;
	int joueur2 = 0;
	int bleugagne = 0;
	int rougegagne = 0;
    SDL_Event event;
    
    SDL_Surface* image = IMG_Load("images/Background.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    
    SDL_Surface* image2 = IMG_Load("images/white.jpg");
	SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, image2);
    
    SDL_Surface* commencer_texte = TTF_RenderText_Solid(font, "JOUER", vert);
	SDL_Texture* commencer_texture = SDL_CreateTextureFromSurface(renderer, commencer_texte);
	SDL_Rect commencerPOS;
	commencerPOS.x = 330;
	commencerPOS.y = 580;
	commencerPOS.w = 200;
	commencerPOS.h = 50;
	
	SDL_Surface* quitter_texte = TTF_RenderText_Solid(font, "QUITTER", vert);
	SDL_Texture* quitter_texture = SDL_CreateTextureFromSurface(renderer, quitter_texte);
	SDL_Rect quitterPOS;
	quitterPOS.x = 330;
	quitterPOS.y = 650;
	quitterPOS.w = 200;
	quitterPOS.h = 50;
	
	SDL_Surface* bleuvsrouge_texte = TTF_RenderText_Solid(font, "BLEU VS ROUGE", vert);
	SDL_Texture* bleuvsrouge_texture = SDL_CreateTextureFromSurface(renderer, bleuvsrouge_texte);
	SDL_Rect bleuvsrougePOS;
	bleuvsrougePOS.x = 330;
	bleuvsrougePOS.y = 580;
	bleuvsrougePOS.w = 200;
	bleuvsrougePOS.h = 50;
	
	SDL_Surface* bleuvsia_texte = TTF_RenderText_Solid(font, "BLEU VS IA", vert);
	SDL_Texture* bleuvsia_texture = SDL_CreateTextureFromSurface(renderer, bleuvsia_texte);
	SDL_Rect bleuvsiaPOS;
	bleuvsiaPOS.x = 330;
	bleuvsiaPOS.y = 650;
	bleuvsiaPOS.w = 200;
	bleuvsiaPOS.h = 50;
	
	SDL_Surface* niveau1_bleuvsrouge_texte = TTF_RenderText_Solid(font, "NIVEAU I", vert);
	SDL_Texture* niveau1_bleuvsrouge_texture = SDL_CreateTextureFromSurface(renderer, niveau1_bleuvsrouge_texte);
	SDL_Rect niveau1_bleuvsrougePOS;
	niveau1_bleuvsrougePOS.x = 330;
	niveau1_bleuvsrougePOS.y = 580;
	niveau1_bleuvsrougePOS.w = 200;
	niveau1_bleuvsrougePOS.h = 50;
	
	SDL_Surface* niveau2_bleuvsrouge_texte = TTF_RenderText_Solid(font, "NIVEAU II", vert);
	SDL_Texture* niveau2_bleuvsrouge_texture = SDL_CreateTextureFromSurface(renderer, niveau2_bleuvsrouge_texte);
	SDL_Rect niveau2_bleuvsrougePOS;
	niveau2_bleuvsrougePOS.x = 330;
	niveau2_bleuvsrougePOS.y = 650;
	niveau2_bleuvsrougePOS.w = 200;
	niveau2_bleuvsrougePOS.h = 50;
	
	SDL_Surface* bleugagne_texte = TTF_RenderText_Solid(font, "BLEU GAGNE", vert);
	SDL_Texture* bleugagne_texture = SDL_CreateTextureFromSurface(renderer, bleugagne_texte);
	SDL_Rect bleugagnePOS;
	bleugagnePOS.x = 330;
	bleugagnePOS.y = 580;
	bleugagnePOS.w = 200;
	bleugagnePOS.h = 50;
	
	SDL_Surface* rougegagne_texte = TTF_RenderText_Solid(font, "ROUGE GAGNE", vert);
	SDL_Texture* rougegagne_texture = SDL_CreateTextureFromSurface(renderer, rougegagne_texte);
	SDL_Rect rougegagnePOS;
	rougegagnePOS.x = 330;
	rougegagnePOS.y = 580;
	rougegagnePOS.w = 200;
	rougegagnePOS.h = 50;
	
	SDL_Surface* pion = IMG_Load("sprites/pion.png");
	SDL_Rect rect_pion;
	rect_pion.x = 346;
	rect_pion.y = 554;
	rect_pion.w = 55;
	rect_pion.h = 85;
	SDL_Texture* texture_pion = SDL_CreateTextureFromSurface(renderer, pion);
	
	SDL_Surface* pion2 = IMG_Load("sprites/pion2.png");
	SDL_Rect rect_pion2;
	rect_pion2.x = 452;
	rect_pion2.y = 96;
	rect_pion2.w = 55;
	rect_pion2.h = 85;
	SDL_Texture* texture_pion2 = SDL_CreateTextureFromSurface(renderer, pion2);
	
	SDL_Surface* fou = IMG_Load("sprites/fou.png");
	SDL_Rect rect_fou;
	rect_fou.x = 240;
	rect_fou.y = 649;
	rect_fou.w = 55;
	rect_fou.h = 85;
	SDL_Texture* texture_fou = SDL_CreateTextureFromSurface(renderer, fou);
	
	SDL_Surface* fou2 = IMG_Load("sprites/fou2.png");
	SDL_Rect rect_fou2;
	rect_fou2.x = 560;
	rect_fou2.y = 2;
	rect_fou2.w = 55;
	rect_fou2.h = 85;
	SDL_Texture* texture_fou2 = SDL_CreateTextureFromSurface(renderer, fou2);
	
	SDL_Surface* cavalier = IMG_Load("sprites/cavalier.png");
	SDL_Rect rect_cavalier;
	rect_cavalier.x = 669;
	rect_cavalier.y = 649;
	rect_cavalier.w = 55;
	rect_cavalier.h = 85;
	SDL_Texture* texture_cavalier = SDL_CreateTextureFromSurface(renderer, cavalier);
	
	SDL_Surface* cavalier2 = IMG_Load("sprites/cavalier2.png");
	SDL_Rect rect_cavalier2;
	rect_cavalier2.x = 130;
	rect_cavalier2.y = 2;
	rect_cavalier2.w = 55;
	rect_cavalier2.h = 85;
	SDL_Texture* texture_cavalier2 = SDL_CreateTextureFromSurface(renderer, cavalier2);
	
	SDL_Surface* tour = IMG_Load("sprites/tour.png");
	SDL_Rect rect_tour;
	rect_tour.x = 22;
	rect_tour.y = 649;
	rect_tour.w = 55;
	rect_tour.h = 85;
	SDL_Texture* texture_tour = SDL_CreateTextureFromSurface(renderer, tour);
	
	SDL_Surface* tour2 = IMG_Load("sprites/tour2.png");
	SDL_Rect rect_tour2;
	rect_tour2.x = 777;
	rect_tour2.y = 2;
	rect_tour2.w = 55;
	rect_tour2.h = 85;
	SDL_Texture* texture_tour2 = SDL_CreateTextureFromSurface(renderer, tour2);
	
	SDL_Surface* reine = IMG_Load("sprites/reine.png");
	SDL_Rect rect_reine;
	rect_reine.x = 455;
	rect_reine.y = 647;
	rect_reine.w = 55;
	rect_reine.h = 85;
	SDL_Texture* texture_reine = SDL_CreateTextureFromSurface(renderer, reine);
	
	SDL_Surface* reine2 = IMG_Load("sprites/reine2.png");
	SDL_Rect rect_reine2;
	rect_reine2.x = 346;
	rect_reine2.y = 3;
	rect_reine2.w = 55;
	rect_reine2.h = 85;
	SDL_Texture* texture_reine2 = SDL_CreateTextureFromSurface(renderer, reine2);
	
	int pion1_click = 0;
	int fou1_click = 0;
	int cavalier1_click = 0;
	int tour1_click = 0;
	int reine1_click = 0;
	
	int pion2_click = 0;
	int fou2_click = 0;
	int cavalier2_click = 0;
	int tour2_click = 0;
	int reine2_click = 0;
	
	int pion1_dead = 1;
	int fou1_dead = 1;
	int cavalier1_dead = 1;
	int tour1_dead = 1;
	int reine1_dead = 1;
	
	int pion2_dead = 1;
	int fou2_dead = 1;
	int cavalier2_dead = 1;
	int tour2_dead = 1;
	int reine2_dead = 1;
	
	srand (time(NULL));
	
    //MENU
    while(continuer)
    {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, commencer_texture, NULL, &commencerPOS);
        SDL_RenderCopy(renderer, quitter_texture, NULL, &quitterPOS);
		
    	if(SDL_PollEvent(&event))
    	{
    		if (event.type == SDL_QUIT) 
			{
				continuer = 0;
				difficulter = 0;
				bleuvsia = 0;
				choix = 0;
				niveau1_bleuvsrouge = 0;
				niveau2_bleuvsrouge = 0;
			}
			
			if (event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
				{
					continuer = 0;
					difficulter = 0;
					bleuvsia = 0;
					choix = 0;
					niveau1_bleuvsrouge = 0;
					niveau2_bleuvsrouge = 0;
				}
			}
			
			//Boutton QUITTER
			if(event.button.x >= 330 && event.button.y >= 650 && event.button.x <= 530 && event.button.y <= 690)
			{
				Mix_PlayChannel( -1, cursomove, 0 );
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						Mix_PlayChannel( -1, select, 0 );
						continuer = 0;
						difficulter = 0;
						bleuvsia = 0;
						choix = 0;
						niveau1_bleuvsrouge = 0;
						niveau2_bleuvsrouge = 0;
						std::cout << "Quitter \n";
					}		
				}
			}
			
			//Boutton JOUER
			if(event.button.x >= 330 && event.button.y >= 580 && event.button.x <= 530 && event.button.y <= 623)
			{
				Mix_PlayChannel( -1, cursomove, 0 );
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						Mix_PlayChannel( -1, select, 0 );
						SDL_DestroyTexture(texture);
						SDL_FreeSurface(image);
						
						SDL_DestroyTexture(commencer_texture);
						SDL_FreeSurface(commencer_texte);
						    
						SDL_DestroyTexture(quitter_texture);
						SDL_FreeSurface(quitter_texte);
						continuer = 0;
					}
				}
			}
		}
		SDL_RenderPresent(renderer);
	}
	
	while(difficulter)
	{
	   	SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   	SDL_RenderCopy(renderer, bleuvsrouge_texture, NULL, &bleuvsrougePOS);
        SDL_RenderCopy(renderer, bleuvsia_texture, NULL, &bleuvsiaPOS);
	   	SDL_RenderPresent(renderer);
	   	
	   	if(SDL_PollEvent(&event))
    	{
    		if (event.type == SDL_QUIT) 
			{
				difficulter = 0;
				bleuvsia = 0;
				choix = 0;
				niveau1_bleuvsrouge = 0;
				niveau2_bleuvsrouge = 0;
			}
			
			if (event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
				{
					difficulter = 0;
					bleuvsia = 0;
					choix = 0;
					niveau1_bleuvsrouge = 0;
					niveau2_bleuvsrouge = 0;
				}
			}
			
			//Boutton BLEU VS ROUGE
			if(event.button.x >= 330 && event.button.y >= 580 && event.button.x <= 530 && event.button.y <= 623)
			{
				Mix_PlayChannel( -1, cursomove, 0 );
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						Mix_PlayChannel( -1, select, 0 );
						Mix_FreeChunk(cursomove);
						SDL_DestroyTexture(texture);
						SDL_FreeSurface(image);
						
						SDL_DestroyTexture(bleuvsrouge_texture);
						SDL_FreeSurface(bleuvsrouge_texte);
						    
						SDL_DestroyTexture(bleuvsia_texture);
						SDL_FreeSurface(bleuvsia_texte);
						difficulter = 0;
						bleuvsia = 0;
					}
				}
			}
			
			//Boutton BLEU VS IA
			if(event.button.x >= 330 && event.button.y >= 650 && event.button.x <= 530 && event.button.y <= 690)
			{
				Mix_PlayChannel( -1, cursomove, 0 );
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						Mix_PlayChannel( -1, select, 0 );
						Mix_FreeChunk(cursomove);
						SDL_DestroyTexture(texture);
						SDL_FreeSurface(image);
						
						SDL_DestroyTexture(bleuvsrouge_texture);
						SDL_FreeSurface(bleuvsrouge_texte);
						    
						SDL_DestroyTexture(bleuvsia_texture);
						SDL_FreeSurface(bleuvsia_texte);
						difficulter = 0;
						choix = 0;
						niveau1_bleuvsrouge = 0;
						niveau2_bleuvsrouge = 0;
					}		
				}
			}
		}
	}
	
	while(choix)
	{
		SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   	SDL_RenderCopy(renderer, niveau1_bleuvsrouge_texture, NULL, &niveau1_bleuvsrougePOS);
        SDL_RenderCopy(renderer, niveau2_bleuvsrouge_texture, NULL, &niveau2_bleuvsrougePOS);
	   	SDL_RenderPresent(renderer);
	   	
	   	if(SDL_PollEvent(&event))
    	{
    		if (event.type == SDL_QUIT) 
			{
				choix = 0;
				niveau1_bleuvsrouge = 0;
				niveau2_bleuvsrouge = 0;
			}
			
			if (event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
				{
					choix = 0;
					niveau1_bleuvsrouge = 0;
					niveau2_bleuvsrouge = 0;
				}
			}
			
			//Boutton NIVEAU 2
			if(event.button.x >= 330 && event.button.y >= 650 && event.button.x <= 530 && event.button.y <= 690)
			{
				Mix_PlayChannel( -1, cursomove, 0 );
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						Mix_PlayChannel( -1, select, 0 );
						choix = 0;
						niveau1_bleuvsrouge = 0;
						niveau2_bleuvsrouge = 1;
					}		
				}
			}
			
			//Boutton NIVEAU 1
			if(event.button.x >= 330 && event.button.y >= 580 && event.button.x <= 530 && event.button.y <= 623)
			{
				Mix_PlayChannel( -1, cursomove, 0 );
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						Mix_PlayChannel( -1, select, 0 );
						choix = 0;
						niveau1_bleuvsrouge = 1;
						niveau2_bleuvsrouge = 0;
					}
				}
			}
		}
	}
	
	//PARTIE Bleu Vs Rouge (niveau 1)
	while(niveau1_bleuvsrouge)
	{
	   	SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   						
	   	echiquier(renderer);
	   	
	   	//Déclaration des axes X et Y pour la souris
		int x = 0, y = 0; 
		x = event.button.x;
		y = event.button.y;
	
		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) 
			{
				niveau1_bleuvsrouge = 0;
			}
			
			if(joueur1)
			{
				//Touche pour saisir les pièces
				if(event.type == SDL_KEYDOWN)
				{
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
					{
						niveau1_bleuvsrouge = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_Q)
					{
						fou1_click = 1;
						cavalier1_click = 0;
						reine1_click = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_W)
					{
						fou1_click = 0;
						cavalier1_click = 1;
						reine1_click = 0;	
					}
										
					if(event.button.button == SDL_SCANCODE_E)
					{
						fou1_click = 0;
						cavalier1_click = 0;
						reine1_click = 1;	
					}
				}								
				
				//FOU_1
				if(fou1_click)
				{	
					rect_fou.x = x;
					rect_fou.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_fou, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								fou1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								fou1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								fou1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								fou1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								fou1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
							fou1_click = 0;
							joueur1 = 0; 
							joueur2 = 1; 
						}
					}
				}
				
				//CAVALIER_1
				if(cavalier1_click)
				{	
					rect_cavalier.x = x;
					rect_cavalier.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_cavalier, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								cavalier1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_cavalier, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								cavalier1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_cavalier, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								cavalier1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_cavalier, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								cavalier1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_cavalier, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								cavalier1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
							cavalier1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
						}
					}
				}
								
				//REINE_1
				if(reine1_click)
				{	
					rect_reine.x = x;
					rect_reine.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_reine, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								reine1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								reine1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								reine1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								reine1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								reine1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
							reine1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
						}
					}
				}
			}
			
			//JOUEUR2
			if(joueur2)
			{
				//Touche pour saisir les pièces
				if(event.type == SDL_KEYDOWN)
				{
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
					{
						niveau1_bleuvsrouge = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_Q)
					{
						fou2_click = 1;	
						cavalier2_click = 0;
						reine2_click = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_W)
					{
						fou2_click = 0;	
						cavalier2_click = 1;
						reine2_click = 0;	
					}
										
					if(event.button.button == SDL_SCANCODE_E)
					{
						fou2_click = 0;	
						cavalier2_click = 0;
						reine2_click = 1;	
					}
				}
				
				//FOU_2
				if(fou2_click)
				{	
					rect_fou2.x = x;
					rect_fou2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_fou2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								fou2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								fou2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								fou2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								fou2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								fou2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							fou2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
								
				//CAVALIER_2
				if(cavalier2_click)
				{	
					rect_cavalier2.x = x;
					rect_cavalier2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_cavalier2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								cavalier2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								cavalier2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								cavalier2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								cavalier2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								cavalier2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							cavalier2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
								
				//REINE_2
				if(reine2_click)
				{	
					rect_reine2.x = x;
					rect_reine2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_reine2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								reine2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								reine2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								reine2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								reine2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								reine2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							reine2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
			}
		}
		if(fou1_dead == 0 && cavalier1_dead == 0 && reine1_dead == 0)
		{
			niveau1_bleuvsrouge = 0;
			bleugagne = 0;
			rougegagne = 1;	
		}
				
		if(fou2_dead == 0 && cavalier2_dead == 0 && reine2_dead == 0)
		{
			niveau1_bleuvsrouge = 0;
			bleugagne = 1;
			rougegagne = 0;	
		}		
		SDL_RenderCopy(renderer, texture_fou, NULL, &rect_fou);
		SDL_RenderCopy(renderer, texture_cavalier, NULL, &rect_cavalier);
		SDL_RenderCopy(renderer, texture_reine, NULL, &rect_reine);
							
		SDL_RenderCopy(renderer, texture_fou2, NULL, &rect_fou2);
		SDL_RenderCopy(renderer, texture_cavalier2, NULL, &rect_cavalier2);
		SDL_RenderCopy(renderer, texture_reine2, NULL, &rect_reine2);
							
	   	SDL_RenderPresent(renderer);
	}
	
	//PARTIE Bleu Vs Rouge (niveau 2)
	while(niveau2_bleuvsrouge)
	{
	   	SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   						
	   	echiquier(renderer);
	   	
	   	//Déclaration des axes X et Y pour la souris
		int x = 0, y = 0; 
		x = event.button.x;
		y = event.button.y;
	
		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) 
			{
				niveau2_bleuvsrouge = 0;
			}
			
			if(joueur1)
			{
				//Touche pour saisir les pièces
				if(event.type == SDL_KEYDOWN)
				{
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
					{
						niveau2_bleuvsrouge = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_Q)
					{
						pion1_click = 1;	
					}  
										
					if(event.button.button == SDL_SCANCODE_W)
					{
						fou1_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_E)
					{
						cavalier1_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_R)
					{
						tour1_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_T)
					{
						reine1_click = 1;	
					}
				}
									
				//PION_1
				if(pion1_click)
				{
					if(event.type == SDL_MOUSEMOTION)
					{
						rect_pion.x = x;
						rect_pion.y = y;
						std::cout << x << "," << y << "\n";
					}		
					
					if(event.type == SDL_MOUSEBUTTONDOWN)
			   		{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_pion, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								pion1_click = 0;
								pion2_dead = 0;
								joueur1 = 0;
								joueur2 = 1; 
							}
												
							if(collision(rect_pion, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								pion1_click = 0;
								fou2_dead = 0;
								joueur1 = 0;
								joueur2 = 1; 
							}
												
							if(collision(rect_pion, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								pion1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0;
								joueur2 = 1; 
							}
												
							if(collision(rect_pion, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								pion1_click = 0;
								tour2_dead = 0;
								joueur1 = 0;
								joueur2 = 1; 
							}
												
							if(collision(rect_pion, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								pion1_click = 0;
								reine2_dead = 0;
								joueur1 = 0;
								joueur2 = 1; 
							}
							
							pion1_click = 0;
							joueur1 = 0; 
							joueur2 = 1; 
						}
					}
				}
				
				//FOU_1
				if(fou1_click)
				{	
					rect_fou.x = x;
					rect_fou.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_fou, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								fou1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								fou1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								fou1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								fou1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_fou, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								fou1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
							fou1_click = 0;
							joueur1 = 0; 
							joueur2 = 1; 
						}
					}
				}
				
				//CAVALIER_1
				if(cavalier1_click)
				{	
					rect_cavalier.x = x;
					rect_cavalier.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_cavalier, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								cavalier1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1; 
							}
											
							if(collision(rect_cavalier, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								cavalier1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_cavalier, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								cavalier1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_cavalier, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								cavalier1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_cavalier, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								cavalier1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
							cavalier1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
						}
					}
				}
								
				//TOUR_1
				if(tour1_click)
				{	
					rect_tour.x = x;
					rect_tour.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_tour, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								tour1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_tour, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								tour1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_tour, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								tour1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_tour, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								tour1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_tour, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								tour1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
							tour1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
						}
					}
				}
								
				//REINE_1
				if(reine1_click)
				{	
					rect_reine.x = x;
					rect_reine.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_reine, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								reine1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								reine1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								reine1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								reine1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
											
							if(collision(rect_reine, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								reine1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
							}
							reine1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
						}
					}
				}
			}
			
			//JOUEUR2
			if(joueur2)
			{
				//Touche pour saisir les pièces
				if(event.type == SDL_KEYDOWN)
				{
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
					{
						niveau2_bleuvsrouge = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_Q)
					{
						pion2_click = 1;	
					}  
										
					if(event.button.button == SDL_SCANCODE_W)
					{
						fou2_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_E)
					{
						cavalier2_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_R)
					{
						tour2_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_T)
					{
						reine2_click = 1;	
					}
				}
										
				//PION_2
				if(pion2_click)
			  	{	
					if(event.type == SDL_MOUSEMOTION)
					{
						rect_pion2.x = x;
						rect_pion2.y = y;
					}
										
					if(event.type == SDL_MOUSEBUTTONDOWN)
			   		{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_pion2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								pion2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
												
							if(collision(rect_pion2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								pion2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
												
							if(collision(rect_pion2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								pion2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
												
							if(collision(rect_pion2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								pion2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
												
							if(collision(rect_pion2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								pion2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							pion2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
				
				//FOU_2
				if(fou2_click)
				{	
					rect_fou2.x = x;
					rect_fou2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_fou2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								fou2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								fou2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								fou2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								fou2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_fou2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								fou2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							fou2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
								
				//CAVALIER_2
				if(cavalier2_click)
				{	
					rect_cavalier2.x = x;
					rect_cavalier2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_cavalier2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								cavalier2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								cavalier2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								cavalier2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								cavalier2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_cavalier2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								cavalier2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							cavalier2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
								
				//TOUR_2
				if(tour2_click)
				{	
					rect_tour2.x = x;
					rect_tour2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_tour2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								tour2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_tour2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								tour2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_tour2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								tour2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_tour2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								tour2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_tour2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								tour2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							tour2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
								
				//REINE_2
				if(reine2_click)
				{	
					rect_reine2.x = x;
					rect_reine2.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_reine2, rect_pion))
							{
								SDL_DestroyTexture(texture_pion);
								reine2_click = 0;
								pion1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_fou))
							{
								SDL_DestroyTexture(texture_fou);
								reine2_click = 0;
								fou1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_cavalier))
							{
								SDL_DestroyTexture(texture_cavalier);
								reine2_click = 0;
								cavalier1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_tour))
							{
								SDL_DestroyTexture(texture_tour);
								reine2_click = 0;
								tour1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
											
							if(collision(rect_reine2, rect_reine))
							{
								SDL_DestroyTexture(texture_reine);
								reine2_click = 0;
								reine1_dead = 0;
								joueur1 = 1; 
								joueur2 = 0; 
							}
							reine2_click = 0;
							joueur1 = 1; 
							joueur2 = 0; 
						}
					}
				}
			}
		}
		if(pion1_dead == 0 && fou1_dead == 0 && cavalier1_dead == 0 && tour1_dead == 0 && reine1_dead == 0)
		{
			niveau2_bleuvsrouge = 0;
			bleugagne = 0;
			rougegagne = 1;	
		}
				
		if(pion2_dead == 0 && fou2_dead == 0 && cavalier2_dead == 0 && tour2_dead == 0 && reine2_dead == 0)
		{
			niveau2_bleuvsrouge = 0;
			bleugagne = 1;
			rougegagne = 0;	
		}		
		SDL_RenderCopy(renderer, texture_pion, NULL, &rect_pion);
		SDL_RenderCopy(renderer, texture_fou, NULL, &rect_fou);
		SDL_RenderCopy(renderer, texture_cavalier, NULL, &rect_cavalier);
		SDL_RenderCopy(renderer, texture_tour, NULL, &rect_tour);
		SDL_RenderCopy(renderer, texture_reine, NULL, &rect_reine);
							
		SDL_RenderCopy(renderer, texture_pion2, NULL, &rect_pion2);
		SDL_RenderCopy(renderer, texture_fou2, NULL, &rect_fou2);
		SDL_RenderCopy(renderer, texture_cavalier2, NULL, &rect_cavalier2);
		SDL_RenderCopy(renderer, texture_tour2, NULL, &rect_tour2);
		SDL_RenderCopy(renderer, texture_reine2, NULL, &rect_reine2);
							
	   	SDL_RenderPresent(renderer);
	}
	
	//Partie Bleu vs IA
	while(bleuvsia)
	{
	   	SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   						
	   	echiquier(renderer);
	   	
	   	//Déclaration des axes X et Y pour la souris
		int x = 0, y = 0; 
		x = event.button.x;
		y = event.button.y;
		
		int nbrandom;
	
		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) 
			{
				bleuvsia = 0;
			}
			
			if(joueur1)
			{
				//Touche pour saisir les pièces
				if(event.type == SDL_KEYDOWN)
				{
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)				
					{
						bleuvsia = 0;
					}
										
					if(event.button.button == SDL_SCANCODE_Q)
					{
						pion1_click = 1;
					}  
										
					if(event.button.button == SDL_SCANCODE_W)
					{
						fou1_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_E)
					{
						cavalier1_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_R)
					{
						tour1_click = 1;	
					}
										
					if(event.button.button == SDL_SCANCODE_T)
					{
						reine1_click = 1;	
					}
				}
									
				//PION_1
				if(pion1_click)
				{
					if(event.type == SDL_MOUSEMOTION)
					{
						rect_pion.x = x;
						rect_pion.y = y;
					}		
					
					if(event.type == SDL_MOUSEBUTTONDOWN)
			   		{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_pion, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								pion1_click = 0;
								pion2_dead = 0;
								joueur1 = 0;
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 1)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
												
							if(collision(rect_pion, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								pion1_click = 0;
								fou2_dead = 0;
								joueur1 = 0;
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 2)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
												
							if(collision(rect_pion, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								pion1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0;
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 3)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
												
							if(collision(rect_pion, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								pion1_click = 0;
								tour2_dead = 0;
								joueur1 = 0;
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 4)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
												
							if(collision(rect_pion, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								pion1_click = 0;
								reine2_dead = 0;
								joueur1 = 0;
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 5)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
							pion1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
							nbrandom = (rand() %5) + 1;
						}
					}
				}
				
				//FOU_1
				if(fou1_click)
				{	
					rect_fou.x = x;
					rect_fou.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_fou, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								fou1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 1)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_fou, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								fou1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 2)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_fou, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								fou1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 3)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_fou, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								fou1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 4)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_fou, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								fou1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 5)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
							fou1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
							nbrandom = (rand() %5) + 1;
						}
					}
				}
				
				//CAVALIER_1
				if(cavalier1_click)
				{	
					rect_cavalier.x = x;
					rect_cavalier.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_cavalier, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								cavalier1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 1)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_cavalier, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								cavalier1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 2)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_cavalier, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								cavalier1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 3)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_cavalier, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								cavalier1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 4)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_cavalier, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								cavalier1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 5)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
							cavalier1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
							nbrandom = (rand() %5) + 1;
						}
					}
				}
								
				//TOUR_1
				if(tour1_click)
				{	
					rect_tour.x = x;
					rect_tour.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_tour, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								tour1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 1)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_tour, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								tour1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 2)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_tour, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								tour1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 3)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_tour, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								tour1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 4)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_tour, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								tour1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 5)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
							tour1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
							nbrandom = (rand() %5) + 1;
						}
					}
				}
								
				//REINE_1
				if(reine1_click)
				{	
					rect_reine.x = x;
					rect_reine.y = y;
					if(event.type == SDL_MOUSEBUTTONDOWN)
		   			{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(collision(rect_reine, rect_pion2))
							{
								SDL_DestroyTexture(texture_pion2);
								reine1_click = 0;
								pion2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 1)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_reine, rect_fou2))
							{
								SDL_DestroyTexture(texture_fou2);
								reine1_click = 0;
								fou2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 2)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_reine, rect_cavalier2))
							{
								SDL_DestroyTexture(texture_cavalier2);
								reine1_click = 0;
								cavalier2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 3)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_reine, rect_tour2))
							{
								SDL_DestroyTexture(texture_tour2);
								reine1_click = 0;
								tour2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 4)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
											
							if(collision(rect_reine, rect_reine2))
							{
								SDL_DestroyTexture(texture_reine2);
								reine1_click = 0;
								reine2_dead = 0;
								joueur1 = 0; 
								joueur2 = 1;
								nbrandom = (rand() %5) + 1;
								while(nbrandom == 5)
								{
									nbrandom = (rand() %5) + 1;
								}
							}
							reine1_click = 0;
							joueur1 = 0; 
							joueur2 = 1;
							nbrandom = (rand() %5) + 1;
						}
					}
				}
			}
			
			//JOUEUR2
			if(joueur2)
			{										
				//PION_2
				if(nbrandom == 1)
			  	{	
					rect_pion2.y = rect_pion2.y + 92;
					
					if(collision(rect_pion2, rect_pion))
					{
						SDL_DestroyTexture(texture_pion);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_pion2, rect_fou))
					{
						SDL_DestroyTexture(texture_fou);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_pion2, rect_cavalier))
					{
						SDL_DestroyTexture(texture_cavalier);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_pion2, rect_tour))
					{
						SDL_DestroyTexture(texture_tour);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_pion2, rect_reine))
					{
						SDL_DestroyTexture(texture_reine);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					joueur1 = 1; 
					joueur2 = 0;
				}
				
				//FOU_2
				if(nbrandom == 2)
				{
					int nbfou = (rand() %2) + 1;
					
					if(nbfou == 1)
					{
						rect_fou2.y = rect_fou2.y + 92;
						rect_fou2.x = rect_fou2.x + 107;	
					}						
					
					if(nbfou == 2)
					{
						rect_fou2.y = rect_fou2.y + 92;
						rect_fou2.x = rect_fou2.x - 107;	
					}
					
					if(collision(rect_fou2, rect_pion))
					{
						SDL_DestroyTexture(texture_pion);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_fou2, rect_fou))
					{
						SDL_DestroyTexture(texture_fou);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_fou2, rect_cavalier))
					{
						SDL_DestroyTexture(texture_cavalier);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_fou2, rect_tour))
					{
						SDL_DestroyTexture(texture_tour);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_fou2, rect_reine))
					{
						SDL_DestroyTexture(texture_reine);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					joueur1 = 1; 
					joueur2 = 0;
				}
								
				//CAVALIER_2
				if(nbrandom == 3)
				{	
					rect_cavalier2.x = rect_cavalier2.x + 107;
					rect_cavalier2.y = rect_cavalier2.y + 184;
					
					if(collision(rect_cavalier2, rect_pion))
					{
						SDL_DestroyTexture(texture_pion);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_cavalier2, rect_fou))
					{
						SDL_DestroyTexture(texture_fou);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_cavalier2, rect_cavalier))
					{
						SDL_DestroyTexture(texture_cavalier);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_cavalier2, rect_tour))
					{
						SDL_DestroyTexture(texture_tour);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_cavalier2, rect_reine))
					{
						SDL_DestroyTexture(texture_reine);
						joueur1 = 1; 
						joueur2 = 0;
					}
									
					joueur1 = 1; 
					joueur2 = 0;
				}
								
				//TOUR_2
				if(nbrandom == 4)
				{	
					rect_tour2.y = rect_tour2.y + 92;
					
					if(collision(rect_tour2, rect_pion))
					{
						SDL_DestroyTexture(texture_pion);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_tour2, rect_fou))
					{
						SDL_DestroyTexture(texture_fou);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_tour2, rect_cavalier))
					{
						SDL_DestroyTexture(texture_cavalier);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_tour2, rect_tour))
					{
						SDL_DestroyTexture(texture_tour);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_tour2, rect_reine))
					{
						SDL_DestroyTexture(texture_reine);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					joueur1 = 1; 
					joueur2 = 0;
				}
								
				//REINE_2
				if(nbrandom == 5)
				{	
					rect_reine2.y = rect_reine2.y + 92;
					
					if(collision(rect_reine2, rect_pion))
					{
						SDL_DestroyTexture(texture_pion);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_reine2, rect_fou))
					{
						SDL_DestroyTexture(texture_fou);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_reine2, rect_cavalier))
					{
						SDL_DestroyTexture(texture_cavalier);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_reine2, rect_tour))
					{
						SDL_DestroyTexture(texture_tour);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					if(collision(rect_reine2, rect_reine))
					{
						SDL_DestroyTexture(texture_reine);
						joueur1 = 1; 
						joueur2 = 0;
					}
					
					joueur1 = 1; 
					joueur2 = 0;
				}
			}		
		}
		if(pion1_dead == 0 && fou1_dead == 0 && cavalier1_dead == 0 && tour1_dead == 0 && reine1_dead == 0)
		{
			bleuvsia = 0;
			bleugagne = 0;
			rougegagne = 1;	
		}
				
		if(pion2_dead == 0 && fou2_dead == 0 && cavalier2_dead == 0 && tour2_dead == 0 && reine2_dead == 0)
		{
			bleuvsia = 0;
			bleugagne = 1;
			rougegagne = 0;	
		}
		SDL_RenderCopy(renderer, texture_pion, NULL, &rect_pion);
		SDL_RenderCopy(renderer, texture_fou, NULL, &rect_fou);
		SDL_RenderCopy(renderer, texture_cavalier, NULL, &rect_cavalier);
		SDL_RenderCopy(renderer, texture_tour, NULL, &rect_tour);
		SDL_RenderCopy(renderer, texture_reine, NULL, &rect_reine);
							
		SDL_RenderCopy(renderer, texture_pion2, NULL, &rect_pion2);
		SDL_RenderCopy(renderer, texture_fou2, NULL, &rect_fou2);
		SDL_RenderCopy(renderer, texture_cavalier2, NULL, &rect_cavalier2);
		SDL_RenderCopy(renderer, texture_tour2, NULL, &rect_tour2);
		SDL_RenderCopy(renderer, texture_reine2, NULL, &rect_reine2);
							
	   	SDL_RenderPresent(renderer);
	}
	
	//Bleu gagne
	while(bleugagne)
	{
		SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   	SDL_RenderCopy(renderer, bleugagne_texture, NULL, &bleugagnePOS);
	   	SDL_RenderPresent(renderer);
	   	Mix_PlayChannel( -1, select, 0 );
	   	
	   	if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) 
			{
				bleugagne = 0;
			}
		}
	}
	
	//Rouge gagne
	while(rougegagne)
	{
		SDL_RenderCopy(renderer, texture2, NULL, NULL);
	   	SDL_RenderCopy(renderer, rougegagne_texture, NULL, &rougegagnePOS);
	   	SDL_RenderPresent(renderer);
	   	Mix_PlayChannel( -1, select, 0 );
	   	
	   	if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) 
			{
				rougegagne = 0;
			}
		}
	}
	
	SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    
    SDL_DestroyTexture(commencer_texture);
    SDL_FreeSurface(commencer_texte);
    
    SDL_DestroyTexture(quitter_texture);
    SDL_FreeSurface(quitter_texte);
    
    SDL_DestroyTexture(bleuvsrouge_texture);
	SDL_FreeSurface(bleuvsrouge_texte);
					    
	SDL_DestroyTexture(bleuvsia_texture);
	SDL_FreeSurface(bleuvsia_texte);
	
	SDL_DestroyTexture(niveau1_bleuvsrouge_texture);
	SDL_FreeSurface(niveau1_bleuvsrouge_texte);
					    
	SDL_DestroyTexture(niveau2_bleuvsrouge_texture);
	SDL_FreeSurface(niveau2_bleuvsrouge_texte);
	
	SDL_DestroyTexture(bleugagne_texture);
	SDL_FreeSurface(bleugagne_texte);
	
	SDL_DestroyTexture(rougegagne_texture);
	SDL_FreeSurface(rougegagne_texte);
    
    SDL_DestroyTexture(texture_pion);
    SDL_FreeSurface(pion);
    
    SDL_DestroyTexture(texture_fou);
    SDL_FreeSurface(fou);
    
    SDL_DestroyTexture(texture_cavalier);
    SDL_FreeSurface(cavalier);
    
    SDL_DestroyTexture(texture_tour);
    SDL_FreeSurface(tour);
    
    SDL_DestroyTexture(texture_reine);
    SDL_FreeSurface(reine);
    
    SDL_DestroyTexture(texture_pion2);
    SDL_FreeSurface(pion2);
    
    SDL_DestroyTexture(texture_fou2);
    SDL_FreeSurface(fou2);
    
    SDL_DestroyTexture(texture_cavalier2);
    SDL_FreeSurface(cavalier2);
    
    SDL_DestroyTexture(texture_tour2);
    SDL_FreeSurface(tour2);
    
    SDL_DestroyTexture(texture_reine2);
    SDL_FreeSurface(reine2);
    
    Mix_FreeMusic(music);
    Mix_FreeChunk(select);
    Mix_FreeChunk(cursomove);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font); 
    TTF_Quit();
    Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
