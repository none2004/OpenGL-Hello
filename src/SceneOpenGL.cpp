#include "SceneOpenGL.h"

// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Autres includes

#include <SDL.h>
#include <iostream>
#include <string>
#include "Shader.h"

using namespace glm;

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) : m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
                                                                                             m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0)
{

}


SceneOpenGL::~SceneOpenGL()
{
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}


// Méthodes

bool SceneOpenGL::initialiserFenetre()
{
    // Initialisation de la SDL

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Version d'OpenGL

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // Double Buffer

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Création de la fenêtre

    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(m_fenetre == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Création du contexte OpenGL

    m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

    if(m_contexteOpenGL == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_fenetre);
        SDL_Quit();

        return false;
    }

    return true;
}


bool SceneOpenGL::initGL()
{
    #ifdef WIN32

        // On initialise GLEW

        GLenum initialisationGLEW( glewInit() );


        // Si l'initialisation a échoué :

        if(initialisationGLEW != GLEW_OK)
        {
            // On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)

            std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


            // On quitte la SDL

            SDL_GL_DeleteContext(m_contexteOpenGL);
            SDL_DestroyWindow(m_fenetre);
            SDL_Quit();

            return false;
        }

    #endif


    // Tout s'est bien passé, on retourne true

    return true;
}


void SceneOpenGL::bouclePrincipale()
{
    // Variables

    bool terminer(false);
    float vertices[] = {-0.5, -0.5, -1.0,   0.0, 0.5, -1.0,   0.5, -0.5, -1.0};
    float couleurs[] = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0};


    // Shader

    Shader shaderCouleur("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    shaderCouleur.charger();


    // Matrices

    mat4 projection;
    mat4 modelview;

    projection = perspective(71.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    modelview = mat4(1.0);


    // Boucle principale

    while(!terminer)
    {
        // Gestion des évènements

        SDL_WaitEvent(&m_evenements);

        if(m_evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = true;


        // Nettoyage de l'écran

        glClear(GL_COLOR_BUFFER_BIT);


        // Réinitialisation de la matrice modelview

        modelview = mat4(1.0);


        // On spécifie quel shader utiliser

        glUseProgram(shaderCouleur.getProgramID());


            // On remplie puis on active le tableau Vertex Attrib 0

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);


            // Même chose avec le tableau Vertex Attrib 1

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, couleurs);
            glEnableVertexAttribArray(1);


            // On envoie les matrices au shader

            glUniformMatrix4fv(glGetUniformLocation(shaderCouleur.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glUniformMatrix4fv(glGetUniformLocation(shaderCouleur.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));


            // On affiche le polygone

            glDrawArrays(GL_TRIANGLES, 0, 3);


            // On désactive les tableaux Vertex Attrib puisque l'on n'en a plus besoin

            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);


        // On n'utilise plus le shader

        glUseProgram(0);


        // Actualisation de la fenêtre

        SDL_GL_SwapWindow(m_fenetre);
    }
}
