#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Game.h"

Game::Game() : start(0), last(0), current(0), good(true), running(false),
    width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT),
    particles(std::vector<Particle>())
{
    // Seed the random number generator
    srand(time(0));

    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        good = false;
        return;
    }

    // initialize SDL window
    window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        good = false;
        return;
    }

    // initialize SDL renderer
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        good = false;
        return;
    }

    // initialize particle texture
    SDL_Surface* bmp = SDL_LoadBMP("particle.bmp");
    if (bmp == NULL)
    {
        good = false;
        return;
    }
    particleTexture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (particleTexture == NULL)
    {
        good = false;
        return;
    }

    // initialize our particles
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        particles.push_back(randomParticle());
    }
}

Game::~Game()
{
    if (!good)
    {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }
    if (particleTexture != NULL)
    {
        SDL_DestroyTexture(particleTexture);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int Game::operator()()
{
    if (!good)
    {
        return -1;
    }
    running = true;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    start = SDL_GetTicks();
    last = start;
    while (running) // every iteration is 1 frame
    {
        current = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
        update((current - last) / 1000.0);
        render();
        last = current;
    }
    return 0;
}

void Game::update(double dt)
{
    std::cout << dt << " sec since last frame.\n";
    std::vector<Point> prev;
    double forceX;
    double forceY;
    double force;
    double ang = 0;
    double dist;
    // test loop
    /*
    for(int i = 0; i < PARTICLE_COUNT; i++)
    {
        double temp = particles[i].getPos().getY();
        particles[i].setYPos(temp - 1);
    }
    */

// calculating acceleration/velocity
    for(int i = 0; i< PARTICLE_COUNT; i++)
    {
        force = 0;
        forceX = 0;
        forceY = 0;
        prev.push_back(particles[i].getPos());
        particles[i].setForce(forceX, forceY);
        for(int j = 0; j < PARTICLE_COUNT; j++)
        {
            if(i != j)
            {
                ang = atan2(particles[j].getYPos()-particles[i].getYPos(),
                            particles[j].getXPos()-particles[i].getXPos());
                dist = particles[i].getPos().distanceFrom(particles[j].getPos());
                //  std::cout<<particles[j].getPos().getY()-particles[i].getPos().getY()<<"\n";
                // std::cout<<particles[j].getPos().getX()-particles[i].getPos().getX()<<"\n";
                force = G*particles[i].getMass()*particles[j].getMass()/(dist*dist);
                //std::cout<<force<<"\n";

                forceX += cos(ang) * force;
                forceY += sin(ang) * force;
            }
        }
        // std::cout<<forceX<<"\n";
        //std::cout<<forceY<<"\n";
        //std::cout<<force<<"\n";
        particles[i].setForce(forceX, forceY);
    }

    for(int i = 0; i < PARTICLE_COUNT; i++)
    {
        //std::cout<<particles[0].getVeloc().getY()<< "\n";
        particles[i].updateVel(dt);
        particles[i].updatePos(dt);

    }


    //collision loop
    //make sure collisions aren't calculated twice
    double phi1;
    double phi2;
    double theta1;
    double theta2;
    double xV1 = 0;
    double yV1 = 0;
    double xV2 = 0;
    double yV2 = 0;
    double v1;
    double v2;
    double m1;
    double m2;
    bool collision[PARTICLE_COUNT][PARTICLE_COUNT] = {0};
    for(int i = 0; i< PARTICLE_COUNT; i++)
    {
        for(int j = 0; j < PARTICLE_COUNT; j++)
        {
            if(i != j && !collision[i][j] && particles[i].getPos().distanceFrom(particles[j].getPos()) <= particles[i].getRadius()
                    + particles[j].getRadius())
            {
                m1 = particles[i].getMass();
                m2 = particles[j].getMass();
                collision[i][j] = true;
                collision[j][i] = true;
                particles[i].setXPos(prev[i].getX());
                particles[i].setYPos(prev[i].getY());
                particles[j].setXPos(prev[j].getX());
                particles[j].setYPos(prev[j].getY());
                xV1 = particles[i].getVeloc().getX();
                yV1 = particles[i].getVeloc().getY();
                xV2 = particles[j].getVeloc().getX();
                yV2 = particles[j].getVeloc().getY();
                v1 = pow(xV1*xV1 + yV1*yV1, .5);
                v2 = pow(xV2*xV2 + yV2*yV2, .5);
                phi1 = atan2(particles[j].getYPos()-particles[i].getYPos(),
                             particles[j].getXPos()-particles[i].getXPos());
                phi2 = atan2(particles[i].getYPos()-particles[j].getYPos(),
                             particles[i].getXPos()-particles[j].getXPos());
                theta1 = atan2(particles[i].getVeloc().getY(),particles[i].getVeloc().getX());
                theta2 = atan2(particles[j].getVeloc().getY(),particles[j].getVeloc().getX());

                xV1 = ((v1*cos(theta1-phi1)*(m1-m2) + 2*m2*v2*cos(theta2-phi1))/(m1+m2))
                      *cos(phi1) + v1*sin(theta1-phi1)*cos(phi1 + (M_PI)/2);
                yV1 = ((v1*cos(theta1-phi1)*(m1-m2) + 2*m2*v2*cos(theta2-phi1))/(m1+m2)
                       *sin(phi1) + v1*sin(theta1-phi1)*sin(phi1+ (M_PI)/2));

                xV2 = ((v2*cos(theta2-phi2)*(m2-m1) + 2*m1*v1*cos(theta1-phi2))/(m1+m2))
                      *cos(phi2) + v1*sin(theta2-phi2)*cos(phi2 + (M_PI)/2);
                yV2 = ((v2*cos(theta2-phi2)*(m2-m1) + 2*m1*v1*cos(theta1-phi2))/(m1+m2)
                       *sin(phi2) + v2*sin(theta2-phi2)*sin(phi2+ (M_PI)/2));

                particles[i].setVeloc(xV1,yV1);
                particles[j].setVeloc(xV2,yV2);

            }
        }
    }

    //bounce off walls

    for(int i = 0; i < PARTICLE_COUNT; i++)
    {
        if(particles[i].getPos().getX() >= DEFAULT_WIDTH + particles[i].getRadius())
        {
            particles[i].bounceR(DEFAULT_WIDTH);
        }
        if(particles[i].getPos().getX() <= particles[i].getRadius())
        {
            particles[i].bounceL();
        }

        if(particles[i].getPos().getY() >= DEFAULT_HEIGHT + particles[i].getRadius())
        {
            particles[i].bounceD(DEFAULT_HEIGHT);
        }
        if(particles[i].getPos().getY() <= particles[i].getRadius())
        {
            particles[i].bounceU();
        }
    }

    // Replace with your game logic!
}

void Game::render()
{
    SDL_RenderClear(renderer);

    // rendering here would place objects beneath the particles

    for (const Particle& p : particles)
    {
        drawParticle(p);
    }

    // rendering here would place objects on top of the particles

    SDL_RenderPresent(renderer);
}

void Game::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    // Add your own event handling here if desired
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::drawParticle(const Particle& p)
{
    SDL_Rect dst;

    double shift = p.getRadius();
    dst.x = (int) (p.getPos().getX() - shift);
    dst.y = (int) (p.getPos().getY() - shift);
    dst.w = shift * 2;
    dst.h = shift * 2;
    SDL_RenderCopy(renderer, particleTexture, NULL, &dst);
}

Particle Game::randomParticle() const
{
    Point pos(rand() % width, rand() % height);
    // using some percentage of the mass of Jupiter
    double mass = ((double) rand() / RAND_MAX) * 30 + 1.0;

    return Particle(pos, mass);
}
