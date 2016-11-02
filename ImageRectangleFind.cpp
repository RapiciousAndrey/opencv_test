#include <opencv2/core/core.hpp> 
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <iostream> 

void findRectangles(IplImage* _image) 
{ 
assert(_image!=0); 

IplImage* bin = cvCreateImage( cvGetSize(_image), IPL_DEPTH_8U, 1); 

// конвертируем в градации серого 
cvConvertImage(_image, bin, CV_BGR2GRAY); 
// находим границы 
cvCanny(bin, bin, 50, 200); 

cvNamedWindow( "bin", 1 ); 
cvShowImage("bin", bin); 

// хранилище памяти для контуров 
CvMemStorage* storage = cvCreateMemStorage(0); 
CvSeq* contours=0; 

// находим контуры 
int contoursCont = cvFindContours( bin, storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0)); 

assert(contours!=0); 

// обходим все контуры 
for( CvSeq* current = contours; current != NULL; current = current->h_next ){ 
// вычисляем площадь и периметр контура 
double area = fabs(cvContourArea(current)); 
double perim = cvContourPerimeter(current); 


if ( area / (perim*perim/16) > 0.9 && area / (perim*perim/16) < 1.1 ){ // в 10% интервале 
// нарисуем контур 
cvDrawContours(_image, current, cvScalar(0, 0, 255), cvScalar(0, 255, 0), -1, 1, 8); 
} 
} 

// освобождаем ресурсы 
cvReleaseMemStorage(&storage); 
cvReleaseImage(&bin); 
} 

int main(int argc, char* argv[]) 
{ 
IplImage *src=0, *dst=0; 

// имя картинки задаётся первым параметром 
char* filename = argc >= 2 ? argv[1] : "sqrs.jpg"; 
// получаем картинку 
src = cvLoadImage(filename, 1); 

printf("[i] image: %s\n", filename); 
assert( src != 0 ); 

// покажем изображение 
cvNamedWindow( "original", 1 ); 
cvShowImage( "original", src ); 

dst = cvCloneImage(src); 

// находим круги на изображении 
findRectangles(dst); 

cvNamedWindow( "circles", 1 ); 
cvShowImage( "circles", dst); 

// ждём нажатия клавиши 
cvWaitKey(0); 

// освобождаем ресурсы 
cvReleaseImage(&src); 
cvReleaseImage(&dst); 
// удаляем окна 
cvDestroyAllWindows(); 
return 0; 
}
