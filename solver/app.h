#ifndef _SOLVER_APP_H_
#define _SOLVER_APP_H_

#include "window.h"

class App : public Window
{
    public:
        App();
        ~App(); 

    private:
        void update() final;
};

#endif // _SOLVER_APP_H_