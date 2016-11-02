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
CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); //cvCaptureFromCAM( 0 ); 
assert( capture ); 

//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);//1280); 
//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);//960); 

// узнаем ширину и высоту кадра 
double width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH); 
double height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT); 
printf("[i] %.0f x %.0f\n", width, height ); 


// имя картинки задаётся первым параметром 

// получаем картинку 
while(true){ 
src = cvQueryFrame( capture ); 


assert( src != 0 ); 

// покажем изображение 
cvNamedWindow( "original", 1 ); 
cvShowImage( "original", src ); 

dst = cvCloneImage(src); 

// находим квадраты и на изображении 
findRectangles(dst); 

cvNamedWindow( "circles", 1 ); 
cvShowImage( "circles", dst); 
char c = cvWaitKey(33); 
if (c == 27) { // нажата ESC 
break; 
} 
// ждём нажатия клавиши 

} 
// освобождаем ресурсы 
cvReleaseCapture( &capture ); 
cvReleaseImage(&src); 
cvReleaseImage(&dst); 
// удаляем окна 
cvDestroyAllWindows(); 
return 0; 
}
