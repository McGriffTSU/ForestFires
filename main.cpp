#include <iostream>
#include <tgmath.h>
#include <fstream>
#include <string>
#include <vector>

int main() {
    //Vectors for data read from csv
    int dataPoints = 0;

    std::vector<int> XData;
    std::vector<int> YData;

    std::vector<std::string> monthData;
    std::vector<std::string> dayData;

    std::vector<double> FFMCData;
    std::vector<double> DMCData;
    std::vector<double> DCData;
    std::vector<double> ISIData;
    std::vector<double> tempData;
    std::vector<double> RHData;
    std::vector<double> windData;
    std::vector<double> rainData;
    std::vector<double> areaData;

    //Read data from csv into vectors
    std::fstream infile("forestfires.csv");
    int X,Y;
    std::string month,day;
    double FFMC,DMC,DC,ISI,temp,RH,wind,rain,area;
    char c;
    while(infile >> X >> Y >> month >> day >>  FFMC >>  DMC >>  DC >> ISI >>  temp >>  RH >>  wind >>  rain >>  area)
    {
        dataPoints++;
        XData.push_back(X);
        YData.push_back(Y);
        monthData.push_back(month);
        dayData.push_back(day);
        FFMCData.push_back(FFMC);
        DMCData.push_back(DMC);
        DCData.push_back(DC);
        ISIData.push_back(ISI);
        tempData.push_back(temp);
        RHData.push_back(RH);
        windData.push_back(wind);
        rainData.push_back(rain);
        areaData.push_back(area);
    }

    /*
     * STANDARDIZATION OF DATA BEGINS HERE
     */

    //Calculate mean for variables (leave out X, Y, month, day)
    double FFMCMean = 0, DMCMean = 0, DCMean = 0, ISIMean = 0, tempMean = 0, RHMean = 0, windMean = 0, rainMean = 0, areaMean = 0;
    for(int i = 0; i < dataPoints; i++)
    {
        FFMCMean += FFMCData[i];
        DMCMean += DMCData[i];
        DCMean += DCData[i];
        ISIMean += ISIData[i];
        tempMean += tempData[i];
        RHMean += RHData[i];
        windMean += windData[i];
        rainMean += rainData[i];
        areaMean += areaData[i];
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
        FFMCSdev += (FFMCData[i] - FFMCMean) * (FFMCData[i] - FFMCMean);
        DMCSdev += (DMCData[i] - DMCMean) * (DMCData[i] - DMCMean);
        DCSdev += (DCData[i] - DCMean) * (DCData[i] - DCMean);
        ISISdev += (ISIData[i] - ISIMean) * (ISIData[i] - ISIMean);
        tempSdev += (tempData[i] - tempMean) * (tempData[i] - tempMean);
        RHSdev += (RHData[i] - RHMean) * (RHData[i] - RHMean);
        windSdev += (windData[i] - windMean) * (windData[i] - windMean);
        rainSdev += (rainData[i] - rainMean) * (rainData[i] - rainMean);
        areaSdev += (areaData[i] - areaMean) * (areaData[i] - areaMean);
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

    //Now assign a new value to each data set based on standard deviations from the mean
    for(int i = 0; i < dataPoints; i++)
    {
        FFMCData[i] = (FFMCData[i] - FFMCMean)/FFMCSdev;
        DMCData[i] = (DMCData[i] - DMCMean)/DMCSdev;
        DCData[i] = (DCData[i] - DCMean)/DCSdev;
        ISIData[i] = (ISIData[i] - ISIMean)/ISISdev;
        tempData[i] = (tempData[i] - tempMean)/tempSdev;
        RHData[i] = (RHData[i] - RHMean)/RHSdev;
        windData[i] = (windData[i] - windMean)/windSdev;
        rainData[i] = (rainData[i] - rainMean)/rainSdev;
        areaData[i] = (areaData[i] - areaMean)/areaSdev;
    }

    /*
     * DATA STANDARDIZATION DONE
     */


    std::cout << " DONE ";
}