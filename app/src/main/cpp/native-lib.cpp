#include <jni.h>
#include <android/log.h>
#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_righere_opencvdemo_MainActivity_splitter(JNIEnv *env, jobject instance,
                                                    jint w, jint h) {

    // TODO
    IplImage* srcImg = cvLoadImage("/sdcard/convexd.jpg", CV_LOAD_IMAGE_COLOR);
    int width = w;
    int height = h;
    int IMAGE_SIZE = srcImg->height * srcImg->widthStep;
    IplImage* dstImg[2];
    IplImage* viewImg[2];

    dstImg[0] = cvCreateImage(cvSize(srcImg->width/2, srcImg->height), srcImg->depth, srcImg->nChannels);
    dstImg[1] = cvCreateImage(cvSize(srcImg->width/2, srcImg->height), srcImg->depth, srcImg->nChannels);
    viewImg[0] = cvCreateImage(cvSize(width, height), srcImg->depth, srcImg->nChannels);
    viewImg[1] = cvCreateImage(cvSize(width, height), srcImg->depth, srcImg->nChannels);

    for(int i = 0; i < srcImg->height; i++)
    {
        memcpy(dstImg[0]->imageData + (i * dstImg[0]->widthStep), srcImg->imageData + i * srcImg->widthStep,
               dstImg[0]->widthStep);
    }
    for(int i = 0; i < srcImg->height; i++)
    {
        memcpy(dstImg[1]->imageData + (i * dstImg[1]->widthStep), srcImg->imageData + (int)(srcImg->widthStep) * 1 / 2  + i * srcImg->widthStep,
               dstImg[1]->widthStep);
    }
//    cvSetImageROI(srcImg, cvRect(0, 0, srcImg->width*0.5, srcImg->height));
//    cvCopy(srcImg, dstImg[0]);
//    cvSetImageROI(srcImg, cvRect(srcImg->width*0.5, 0, srcImg->width*0.5, srcImg->height));
//    cvCopy(srcImg, dstImg[1]);
//    cvResetImageROI(srcImg);
    cvResize(dstImg[0], viewImg[0], CV_INTER_LINEAR);
    cvResize(dstImg[1], viewImg[1], CV_INTER_LINEAR);
    int size = width * height;
    int* outImgrgb = new int[size];
    for(int i = 0; i <  height * width / 2; i ++)
    {
        outImgrgb[2*i] = 0xff;
        outImgrgb[2*i] = (outImgrgb[2*i] << 8) + viewImg[0]->imageData[2*i * 3 + 2];
        outImgrgb[2*i] = (outImgrgb[2*i] << 8) + viewImg[0]->imageData[2*i * 3 + 1];
        outImgrgb[2*i] = (outImgrgb[2*i] << 8) + viewImg[0]->imageData[2*i * 3];

        outImgrgb[2*i + 1] = 0xff;
        outImgrgb[2*i + 1] = (outImgrgb[2*i + 1] << 8) + viewImg[1]->imageData[(2*i + 1) * 3 + 2];
        outImgrgb[2*i + 1] = (outImgrgb[2*i + 1] << 8) + viewImg[1]->imageData[(2*i + 1) * 3 + 1];
        outImgrgb[2*i + 1] = (outImgrgb[2*i + 1] << 8) + viewImg[1]->imageData[(2*i + 1) * 3];
    }

    cvReleaseImage(dstImg);
    cvReleaseImage(viewImg);
    cvReleaseImage(&srcImg);
    jintArray result = env->NewIntArray(size);
    env->SetIntArrayRegion(result, 0, size, outImgrgb);
    delete []outImgrgb;
    return result;
}