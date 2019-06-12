#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
//#include <chrono>
//#include <ctime>

#define PI 3.1415926

float R,G,B;

int skip_step_ = 1;

using namespace cv;

//std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;

     std::vector<cv::Point> contours;
std::vector<cv::Point> contours_pro;

//using namespace std::chrono;
cv::Mat g_hist;
    FILE *hist;
IplImage tmp=g_hist;
void drawLine(cv::Mat histImage, int bin_w, int i, int height, cv::Mat b_hist, cv::Scalar color)
{
    cv::line(histImage,cv::Point(bin_w*(i - 1), height - cvRound(b_hist.at<float>(i - 1))),cv::Point(bin_w*(i), height - cvRound(b_hist.at<float>(i))),color);
    fprintf(hist,"%d \t  %d\n", bin_w*(i), cvRound(b_hist.at<float>(i)));
    
}

static void mouse_callback(int event, int x, int y, int, void *)
{
	// Al hacer clic sobre la imagen guardamos las coordenadas
	if(event == EVENT_LBUTTONDOWN)
	{
		
		int a = x;
		std::cout<<a<<std::endl;
	}
	if(event == EVENT_RBUTTONDOWN)
	{
		
		int b = x;
		std::cout<<b<<std::endl;
	}
}


int main(int argc, char** argv )
{
  

    //FILE *pts_pro;
    hist = fopen ( "histo.txt", "w+" );
    //pts_pro = fopen ( "points_pro.txt", "w+" );

    
    if (argc =! 2)
      {
       std::cout<<"indique el directorio de las imagenes"<<std::endl;
       return (-1);
      }

    int numSnap = 0;
    std::string numero = "0";
    std::string alta = "alta";
     int n_scan = 10; 
     int poin;



    cv::Mat image,image2;
    float gr,grr,grb;

        // Tamaño del histograma
  int h_width = 320;
  int h_height = 255;

    cv::Mat gray;


    while(1)
    {
    image = cv::imread(argv[1]+numero+".png" ); //cargar la imagen de el disco duro a la memoria RAM
    //image = cv::imread( "entrenador/"+numero+".png" );
    if ( !image.data )
    {
        std::cout<<"No image data"<<std::endl;
        return -1;
    }
    
   cv::Mat res(image.rows,image.cols,CV_8UC3,cv::Scalar::all(0));     
   //cv::Mat histImage(h_height, h_width, CV_8UC3, cv::Scalar(0, 0, 0));
   cv::Mat g(image.rows,image.cols,CV_8UC3,cv::Scalar::all(0));  //creacion del contenedor de la imagen resultante
        cv::Mat histImage(h_height, h_width, CV_8UC3, cv::Scalar(0, 0, 0));


     //std::chrono::time_point<std::chrono::system_clock> start,end;
     //start = std::chrono::system_clock::now();
    
    //std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    

    
    int nl = image.rows;
    int nc = image.cols * image.channels();

    
    
    
    //
        for (int j=0; j<nl;j++)
    {
     uchar* data = image.ptr<uchar>(j);
     uchar* datar = g.ptr<uchar>(j);
     uchar* datagr = res.ptr<uchar>(j);
          for (int i=0; i<nc;i++)
          {
            if(i%3==0)  //acceso a componentes de primercanal
            {
               B = data[i];
               //datar[i] = 0;
            }
            if(i%3==1) //acceso a componentes de segundocanal
            {
               G = data[i];
               //datar[i] = 0;
            }
            if(i%3==2) //acceso a componentes de tercercanal
            {
               R = data[i];
               gr  = G / (R+G+B) * 255;
               grr = R / (R+G+B) * 255;
               grb = B / (R+G+B) * 255;
               datar[i-1] = gr;  //moverse entre los canales dependiendo que color quieres calcular la cromaticidad.
              //if (grb > 90  && grb < 104)  //modelo de color blanco
              //if (gr > 104  && gr < 160 && grr > 40 && grr < 100 && grb > 40 && grb < 100)  //modelo para color verde. 
              //if (gr > 90  && gr < 125 && grr > 60 && grr < 74) 
              //if (gr > 80 && gr < 94)  //modelo para color blanco.
                //if (gr > 94 && gr < 106 || gr > 101 && gr < 122 ) //modelo de color para cancha de afuera
                  if (gr > 94 && gr < 160)
                  { 
                  datagr[i-2] = B;
                  datagr[i-1] = G;
                  datagr[i] = R;
                  //datagr[i/3] = 255;
                  } 
              
            }
          }
    
    }
    

   
  

  //calculo de histograma  
   std::vector<cv::Mat> bgr;
   cv::split(g, bgr);

  int numbins =255;

  float range[] = { 0, 255 };
  const float* histRange = { range };

  //cv::Mat g_hist;

  calcHist(&bgr[1], 1, 0, cv::Mat(), g_hist, 1, &numbins, &histRange);
  

  // Normalizar los histogramas 
  normalize(g_hist, g_hist, 0, h_height, cv::NORM_MINMAX);

  int bin_width = cvRound((float)h_width / (float)numbins);

  // Dibujar cada una de las lineas
   for (int i = 1; i < numbins; i++)
      {
       drawLine(histImage, bin_width, i, h_height, g_hist, cv::Scalar(0, 255, 0));
      }   
    


   cv::imshow("Display Image", image);
   cv::imshow("resultado",g);
   cv::imshow("histograma",histImage);
   cv::imshow("segmentado",res);
   //cv::imshow("homo",imaho);
   setMouseCallback("histograma", mouse_callback);
     numSnap++;
    numero = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnap))->str();
        // if (cv::waitKey(15) == 103)
    contours.clear();
    contours_pro.clear();
    cv::waitKey(0);
    }
    return 0;
}
