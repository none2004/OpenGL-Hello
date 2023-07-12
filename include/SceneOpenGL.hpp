#ifndef SCENEOPENGL_HPP
#define SCENEOPENGL_HPP

#ifdef WIN32
#include <glew.h>
#endif

#include <SDL.h>
#include <iostream>
#include <string>

// Classe

class SceneOpenGL
{
    public:

    SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre);
    ~SceneOpenGL();

    bool initialiserFenetre();
    bool initGL();
    void bouclePrincipale();


    private:

    std::string m_titreFenetre;
    int m_largeurFenetre;
    int m_hauteurFenetre;

    SDL_Window* m_fenetre;
    SDL_GLContext m_contexteOpenGL;
    SDL_Event m_evenements;
};

#endif // SCENEOPENGL_HPP
