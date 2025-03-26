#ifndef _APP_H_
#define _APP_H_

#include <atomic>
#include <future>

#include "configuration.h"
#include "solution.h"
#include "window.h"

class App : public Window
{
    public:
        App();
        ~App(); 

    private:
        void update() final;
        void renderPlots();
        Configuration m_configuration = Configuration();
        Solution m_solution = Solution();

        std::future<void> m_future;
        std::atomic<float> m_progress = 0;
};

#endif // _APP_H_