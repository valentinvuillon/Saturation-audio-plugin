#include <math.h>   


float waveshaping_function(float sample){
    if(sample>=1)
    {
        return 1;
    }else
    {
        if(sample<=-1)
        {
            return -1;
        }else
        {
            return 1.5*sample-0.5*pow(sample, 3);
        }
    
    }
    
}
