#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <functional>
#include <future>
#include <cstdio>
#include <iostream>

class Timer
{
    public:

        Timer(){};

        template <class callable>

        //Método encargado de conectar los slots con los métodos creados para estos.
        void connect(callable&& f)
        {
			std::thread([=]() 
            {
                while(true)
                {
					if(go.load())
						std::invoke(f);
                    std::this_thread::sleep_for(std::chrono::milliseconds(period.load()));
                }
            }).detach();
        };

        //Método que se encarga de iniciar el timer con el valor dado por parámetros.
        void start(int p)
        {
			period.store(p);
			go.store(true);
        };

        //Método que se encarga de parar el timer.
        void stop() { go.store(!go); };

        //Método que dado un entero por parámetros, carga dicho valor en la variable periodo.
		void setPeriod(int p) { period.store(p) ;};
        
    private:
        std::atomic_bool go = false;
		std::atomic_int period = 0;
        
    
};

#endif // TIMER_H
