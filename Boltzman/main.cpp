#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLEW_STATIC
#define WIDTH 512
#define HEIGHT 512
#define MAX_TEMP 100
#define a 0.00002505
#define dh 0.1
#define dt 100

float *pixels;
float *color;
double *temperature;
double *temperature_bufor;

void RenderScene(float * pixels);

void TemperatureToColor(float * color, double * temperature);

void ColorToBitmap(float *color, float *pixels);

void TemperatureCalculation(double * temperature, double * temperature_bufor);

void SwapBufors(double **bufor1, double **bufor2);

void WarunkiPoczatkowe(double *temperature);


int main()
{

	pixels = (float *) calloc(WIDTH*HEIGHT*3,sizeof(float));

	temperature = (double *) calloc(WIDTH*HEIGHT,sizeof(double));

	temperature_bufor = (double *) calloc(WIDTH*HEIGHT,sizeof(double));

	color = (float *) calloc(WIDTH*HEIGHT,sizeof(float));

	WarunkiPoczatkowe(temperature);
	
	glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Analiza Czasowa", nullptr, nullptr);    
  
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
     
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 20; i++) //DOBRY REZULTAT PRZY 20000
	{
		TemperatureCalculation(temperature,temperature_bufor);

		SwapBufors(&temperature,&temperature_bufor);
		WarunkiPoczatkowe(temperature);
	}

		RenderScene(pixels);
		
	TemperatureToColor(color,temperature);

	ColorToBitmap(color,pixels);


		glFlush();

		glfwSwapBuffers(window);

		glfwWaitEvents();
    }

    return 0;
}

void RenderScene(float * pixels)
{
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels);
}

void TemperatureToColor(float * color, double * temperature)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			int index = i*HEIGHT + j; 
			color[index] = (float)(temperature[index]/MAX_TEMP);
		}
	}
}

void ColorToBitmap(float *color, float *pixels)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			int index = i*WIDTH*3+j*3;
			int index_color = i*WIDTH+j;
			pixels[index] = 0;
			pixels[index+1] = color[index_color];
			pixels[index+2] = 0;
		}
	}
}

void TemperatureCalculation(double * temperature, double * temperature_bufor)
{

	for (int i = 0; i < HEIGHT; ++i) 
	{
	for (int j = 0; j < WIDTH; ++j) 
	{
		int index = i*WIDTH+j;

		if( i != 0 && j != 0 && i != HEIGHT-1 && j != WIDTH-1) 
		{
			int lewo = index - 1;
			int prawo = index + 1;
			int gora = index + WIDTH;
			int dol = index - WIDTH;

			temperature_bufor[index] = dt*a*((temperature[lewo]-2*temperature[index]+temperature[prawo])/(dh*dh)+(temperature[gora]-2*temperature[index]+temperature[dol])/(dh*dh)) + temperature[index];

		}else
		{
			temperature_bufor[index] = temperature[index];
		}
	}
}

}

void SwapBufors(double **bufor1, double **bufor2)
{
	double * temp;
	temp = *bufor1;
	*bufor1 = *bufor2;
	*bufor2 = temp;
}


void WarunkiPoczatkowe(double *temperature){
	for (int i = 0; i < WIDTH; i++)
	{ 
		for (int j = 0; j < HEIGHT; j++)
		{		
			if((j > 231 && j < 281) && (i > 231 && i < 281)) //Dodanie temperatury na œrodek pola
			{
				int index = i*WIDTH+j;
				temperature[index]=MAX_TEMP;   
			}
		}
	}
}









