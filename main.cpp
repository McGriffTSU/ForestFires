#include <iostream>
#include <tgmath.h>
#include <fstream>
#include <string>
#include <vector>
struct dataPoint
{
    int X;
    int Y;

    std::string month;
    std::string day;

    double FFMC;
    double DMC;
    double DC;
    double ISI;
    double temp;
    double RH;
    double wind;
    double rain;
    double area;

    dataPoint(int x, int y, std::string m, std::string d, double ffmc, double dmc, double dc, double isi, double t, double rh, double w, double r, double a)
    {
        this->X = x;
        this->Y = y;
        this->month = m;
        this->day = d;
        this->FFMC = ffmc;
        this->DMC = dmc;
        this->DC = dc;
        this->ISI = isi;
        this->temp = t;
        this->RH = rh;
        this->wind = w;
        this->rain = r;
        this->area = a;
    }
};


int main() {
    //Vectors for data read from csv
    int dataPoints = 0;
    std::vector<dataPoint> data;
    std::vector<dataPoint> data_sdev;

    //Read data from csv into vectors
    std::fstream infile("forestfires.csv");
    int X,Y;
    std::string month,day;
    double FFMC,DMC,DC,ISI,temp,RH,wind,rain,area;
    char c;
    while(infile >> X >> Y >> month >> day >>  FFMC >>  DMC >>  DC >> ISI >>  temp >>  RH >>  wind >>  rain >>  area)
    {
        dataPoints++;
        dataPoint *incoming = new dataPoint(X, Y, month, day, FFMC, DMC, DC, ISI,temp, RH, wind, rain, area);
        data.push_back(*incoming);
    }

    /*
     * STANDARDIZATION OF DATA BEGINS HERE
     */

    //Calculate mean for variables (leave out X, Y, month, day)
    double FFMCMean = 0, DMCMean = 0, DCMean = 0, ISIMean = 0, tempMean = 0, RHMean = 0, windMean = 0, rainMean = 0, areaMean = 0;
    for(int i = 0; i < dataPoints; i++)
    {
        FFMCMean += data[i].FFMC;
        DMCMean += data[i].DMC;
        DCMean += data[i].DC;
        ISIMean += data[i].ISI;
        tempMean += data[i].temp;
        RHMean += data[i].RH;
        windMean += data[i].wind;
        rainMean += data[i].rain;
        areaMean += data[i].area;
    }
    FFMCMean /= dataPoints;
    DMCMean /= dataPoints;
    DCMean /= dataPoints;
    ISIMean /= dataPoints;
    tempMean /= dataPoints;
    RHMean /= dataPoints;
    windMean /= dataPoints;
    rainMean /= dataPoints;
    areaMean /= dataPoints;

    //Calc mean of squared differences
    double FFMCSdev = 0, DMCSdev = 0, DCSdev = 0, ISISdev = 0, tempSdev = 0, RHSdev = 0, windSdev = 0, rainSdev = 0, areaSdev = 0;
    for(int i = 0; i < dataPoints; i++)
    {
        FFMCSdev += (data[i].FFMC - FFMCMean) * (data[i].FFMC - FFMCMean);
        DMCSdev += (data[i].DMC - DMCMean) * (data[i].DMC - DMCMean);
        DCSdev += (data[i].DC - DCMean) * (data[i].DC - DCMean);
        ISISdev += (data[i].ISI - ISIMean) * (data[i].ISI - ISIMean);
        tempSdev += (data[i].temp - tempMean) * (data[i].temp - tempMean);
        RHSdev += (data[i].RH - RHMean) * (data[i].RH - RHMean);
        windSdev += (data[i].wind - windMean) * (data[i].wind - windMean);
        rainSdev += (data[i].rain - rainMean) * (data[i].rain - rainMean);
        areaSdev += (data[i].area - areaMean) * (data[i].area - areaMean);
    }
    //Calculate standard deviation for each set of variables
    FFMCSdev = sqrt((FFMCSdev/dataPoints));
    DMCSdev = sqrt((DMCSdev/dataPoints));
    DCSdev = sqrt((DCSdev/dataPoints));
    ISISdev = sqrt((ISISdev/dataPoints));
    tempSdev = sqrt((tempSdev/dataPoints));
    RHSdev = sqrt((RHSdev/dataPoints));
    windSdev = sqrt((windSdev/dataPoints));
    rainSdev = sqrt((rainSdev/dataPoints));
    areaSdev = sqrt((areaSdev/dataPoints));

    //Now assign a new value to data_sdev based on standard deviations from the mean
    //So now we have two data sets, data (raw data) and data_sdev (standardized data)
    for(int i = 0; i < dataPoints; i++)
    {
        int x = data[i].X;
        int y = data[i].Y;
        std::string m = data[i].month;
        std::string d = data[i].day;
        double ffmc = (data[i].FFMC - FFMCMean)/FFMCSdev;
        double dmc = (data[i].DMC - DMCMean)/DMCSdev;
        double dc = (data[i].DC - DCMean)/DCSdev;
        double isi = (data[i].ISI - ISIMean)/ISISdev;
        double t = (data[i].temp - tempMean)/tempSdev;
        double rh = (data[i].RH - RHMean)/RHSdev;
        double w = (data[i].wind - windMean)/windSdev;
        double r = (data[i].rain - rainMean)/rainSdev;
        double a = (data[i].area - areaMean)/areaSdev;
        dataPoint *incoming = new dataPoint(x, y, m, d, ffmc, dmc, dc, isi, t, rh, w, r, a);
        data_sdev.push_back(*incoming);
    }

    /*
     * DATA STANDARDIZATION DONE
     */

    //Detect major fires (area > 50)


    std::cout << " DONE ";
}