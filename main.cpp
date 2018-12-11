#include <iostream>
#include <tgmath.h>
#include <fstream>
#include <string>
#include <vector>
/*
 * This code is a sandwiched together source file of separate data analysis C++ files I made
 * I found it easier for submission to simply smash them all together in a master file that does
 * the data read and sort one time, then all the analysis and output in a row instead of submitting each individual
 * file that I used for each figure and another separate file for the prediction models.
 *
 * If you would rather I submit the distributed version so that each figure can be seen more clearly
 * as a separate piece of code please let me know: Michael Griffith mcg76@txstate.edu
 */

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

    dataPoint()
    {
        this->X = 0;
        this->Y = 0;
        this->month = '0';
        this->day = '0';
        this->FFMC = 0;
        this->DMC = 0;
        this->DC = 0;
        this->ISI = 0;
        this->temp = 0;
        this->RH = 0;
        this->wind = 0;
        this->rain = 0;
        this->area = 0;
    }
};

double dist(dataPoint, dataPoint);

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

    /*
     * OUTPUT DATA FOR FIGURES
     */
    //Detect major fires (area > 50ha)
    std::vector<dataPoint> majorFires;
    for(int i = 0; i < dataPoints; i++)
    {
        if(data[i].area > 50)
        {
            majorFires.push_back(data[i]);
            //Find out what extraordinary conditions accompany big fires
            //std::cout << data_sdev[i].area << " " << data_sdev[i].month << " " << data_sdev[i].ISI <<" " <<  data_sdev[i].DMC <<" " <<  data_sdev[i].DC <<" " <<  data_sdev[i].RH <<" " <<  data_sdev[i].temp <<" " <<  data_sdev[i].wind << std::endl;
        }
    }

    //Average FWI by month
    int month_num[12] = {0};

    double FFMCsum[12] = {0};
    double DMCsum[12] = {0};
    double DCsum[12] = {0};
    double ISIsum[12] = {0};
    double tempsum[12] = {0};

    double FFMCavg[12] = {0};
    double DMCavg[12] = {0};
    double DCavg[12] = {0};
    double ISIavg[12] = {0};
    double tempavg[12] = {0};

    for(int i = 0; i < dataPoints; i++)
    {
        if(data[i].month == "jan")
        {
            month_num[0]++;
            FFMCsum[0] += data[i].FFMC;
            DMCsum[0] += data[i].DMC;
            DCsum[0] += data[i].DC;
            ISIsum[0] += data[i].ISI;
            tempsum[0] += data[i].temp;
        }
        else if(data[i].month == "feb")
        {
            month_num[1]++;
            FFMCsum[1] += data[i].FFMC;
            DMCsum[1] += data[i].DMC;
            DCsum[1] += data[i].DC;
            ISIsum[1] += data[i].ISI;
            tempsum[1] += data[i].temp;
        }
        else if(data[i].month == "mar")
        {
            month_num[2]++;
            FFMCsum[2] += data[i].FFMC;
            DMCsum[2] += data[i].DMC;
            DCsum[2] += data[i].DC;
            ISIsum[2] += data[i].ISI;
            tempsum[2] += data[i].temp;
        }
        else if(data[i].month == "apr")
        {
            month_num[3]++;
            FFMCsum[3] += data[i].FFMC;
            DMCsum[3] += data[i].DMC;
            DCsum[3] += data[i].DC;
            ISIsum[3] += data[i].ISI;
            tempsum[3] += data[i].temp;
        }
        else if(data[i].month == "may")
        {
            month_num[4]++;
            FFMCsum[4] += data[i].FFMC;
            DMCsum[4] += data[i].DMC;
            DCsum[4] += data[i].DC;
            ISIsum[4] += data[i].ISI;
            tempsum[4] += data[i].temp;
        }
        else if(data[i].month == "jun")
        {
            month_num[5]++;
            FFMCsum[5] += data[i].FFMC;
            DMCsum[5] += data[i].DMC;
            DCsum[5] += data[i].DC;
            ISIsum[5] += data[i].ISI;
            tempsum[5] += data[i].temp;
        }
        else if(data[i].month == "jul")
        {
            month_num[6]++;
            FFMCsum[6] += data[i].FFMC;
            DMCsum[6] += data[i].DMC;
            DCsum[6] += data[i].DC;
            ISIsum[6] += data[i].ISI;
            tempsum[6] += data[i].temp;
        }
        else if(data[i].month == "aug")
        {
            month_num[7]++;
            FFMCsum[7] += data[i].FFMC;
            DMCsum[7] += data[i].DMC;
            DCsum[7] += data[i].DC;
            ISIsum[7] += data[i].ISI;
            tempsum[7] += data[i].temp;
        }
        else if(data[i].month == "sep")
        {
            month_num[8]++;
            FFMCsum[8] += data[i].FFMC;
            DMCsum[8] += data[i].DMC;
            DCsum[8] += data[i].DC;
            ISIsum[8] += data[i].ISI;
            tempsum[8] += data[i].temp;
        }
        else if(data[i].month == "oct")
        {
            month_num[9]++;
            FFMCsum[9] += data[i].FFMC;
            DMCsum[9] += data[i].DMC;
            DCsum[9] += data[i].DC;
            ISIsum[9] += data[i].ISI;
            tempsum[9] += data[i].temp;
        }
        else if(data[i].month == "nov")
        {
            month_num[10]++;
            FFMCsum[10] += data[i].FFMC;
            DMCsum[10] += data[i].DMC;
            DCsum[10] += data[i].DC;
            ISIsum[10] += data[i].ISI;
            tempsum[10] += data[i].temp;
        }
        else if(data[i].month == "dec")
        {
            month_num[11]++;
            FFMCsum[11] += data[i].FFMC;
            DMCsum[11] += data[i].DMC;
            DCsum[11] += data[i].DC;
            ISIsum[11] += data[i].ISI;
            tempsum[11] += data[i].temp;
        }
    }

    for(int i = 0; i < 12; i++)
    {
        FFMCavg[i] = FFMCsum[i]/month_num[i];
        DMCavg[i] = DMCsum[i]/month_num[i];
        DCavg[i] = DCsum[i]/month_num[i];
        ISIavg[i] = ISIsum[i]/month_num[i];
        tempavg[i] = tempsum[i]/month_num[i];
    }

    for(int i = 0; i < 12; i++)
    {
        std::cout << i << " AVERAGES:\t" << FFMCavg[i] << "\t"<< DMCavg[i] << "\t "<< DCavg[i] << " \t"<< ISIavg[i] << "\t "<< tempavg[i] << "\n";
        std::cout << i << " SDEV:\t" << (FFMCavg[i]-FFMCMean)/FFMCSdev << "\t"<< (DMCavg[i] - DMCMean)/DMCSdev << "\t "<< (DCavg[i]-DCMean)/DCSdev << " \t"<< (ISIavg[i] - ISIMean)/ISISdev
        << "\t "<< (tempavg[i] - tempMean)/tempSdev << "\n";
    }

    /*
     * PREDICTION ALGORITHMS
     * KNN - Classification (5-NN)
     * K-fold cross-validation (10-fold)
     */
    //Set up K data points to use
    unsigned int k = 50;
    unsigned int nn = 5;

    std::vector<dataPoint> kcv;
    std::vector<dataPoint> kcv_sdev;
    for(int i = dataPoints-1; i >= dataPoints-k; i--)
    {
        kcv.push_back(data[i]);
        kcv_sdev.push_back(data_sdev[i]);
    }

    //For storing the nearest neighbors
    double kcv_dist[k][nn] = {0};
    std::vector<std::vector<dataPoint>> neighbors(k, std::vector<dataPoint>(nn));

    double kcv_sdev_dist[k][nn] = {0};
    std::vector<std::vector<dataPoint>> sdev_neighbors(k, std::vector<dataPoint>(nn));

    //Use KNN to predict intensity of the fire of the first 10 points
    //Use raw data and standardized data for prediction, see which one is more successful
    for(int j = 0; j < k; j++) {
        for (int i = 0; i < dataPoints; i++) {
            double next = dist(kcv[j], data[i]);
            double next_sdev = dist(kcv_sdev[j], data_sdev[i]);
            for(int x = 0; x < nn; x++)
            {
                if(kcv_dist[j][x] == 0 || kcv_dist[j][x] > next_sdev)
                {
                    kcv_dist[j][x] = next_sdev;
                    neighbors[j][x] = data[i];
                }
            }
        }
    }

    //Now that we have the neighbors, calculate the mean of the neighbors for prediction
    double kcv_sum[k] = {0};
    for(int i = 0; i < k; i++)
    {
        for(int j = 0; j < nn; j++)
        {
            kcv_sum[i] += neighbors[i][j].area;
        }
    }
    //After getting sum of the areas for each K-fold points nearest neighbors, print out
    //the raw average and the standard deviations from the mean
    double kcv_avg[k] = {0};
    double kcv_sdev_avg[k] = {0};
    int low = 0;
    int moderatelow = 0;
    int moderate = 0;
    int severe = 0;
    for(int i = 0; i < k; i++)
    {
        std::string predicted;
        kcv_avg[i] = kcv_sum[i]/nn;
        kcv_sdev_avg[i] = (kcv_avg[i] - areaMean)/areaSdev;
        if(kcv_sdev_avg[i] < -0.1)
        {
            low++;
            predicted = "low ";
        }
        else if(kcv_sdev_avg[i] > -0.1 && kcv_sdev_avg[i] <= 0.05)
        {
            moderatelow++;
            predicted = "mod-low ";
        }
        else if(kcv_sdev_avg[i] > 0.05 && kcv_sdev_avg[i] < 0.2)
        {
            moderate++;
            predicted = "mod ";
        }
        else
        {
            severe++;
            predicted = "sev ";
        }

        std::cout<< i << predicted << "PREDICTION: " << kcv_avg[i] << "\t" << kcv_sdev_avg[i] << '\n';
    }
    std::cout << "\n\nLOW RISK DAYS: " << low << "\n";
    std::cout << "MODERATE-LOW RISK DAYS: " << moderatelow << "\n";
    std::cout << "MODERATE RISK DAYS: " << moderate << "\n";
    std::cout << "SEVERE RISK DAYS: " << severe << "\n";
}

double dist(dataPoint A, dataPoint B)
{
    double xdiff = A.FFMC - B.FFMC;
     xdiff = xdiff * xdiff;

    double ydiff = A.DMC = B.DMC;
    ydiff = ydiff * ydiff;

    double zdiff = A.DC - B.DC;
    zdiff = zdiff * zdiff;

    double wdiff = A.ISI - B.ISI;
    wdiff = wdiff * wdiff;

    double diff = xdiff + ydiff + zdiff + wdiff;
    diff = sqrt(diff);

    return diff;
}