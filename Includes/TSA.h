#pragma once

#include "DataFrame.h"
#include "Ops.h"
#include "../ctsa-master/header/ctsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace adsl {

    void mimicAutoArimaTest1() {
        int i, N, d, D, L;
        double *inp;
        int p, q, P, Q, s, r;
        double *xpred, *amse;
        auto_arima_object obj;
        /*
        Make sure all the parameter values are correct and consistent with other values. eg., if xreg is NULL r should be 0
        or if P = D = Q = 0 then make sure that s is also 0. 
        Recheck the values if the program fails to execute.
        */
        p = 5;
        d = 2;
        q = 5;
        P = 2;
        D = 1;
        Q = 2;
        
        int order[3] = {p,d,q};
        int seasonal[3] = {P,D,Q};

        s = 0;
        r = 0;
        L = 5;

        xpred = (double*)malloc(sizeof(double)* L);
        amse = (double*)malloc(sizeof(double)* L);

        FILE *ifp;
        double temp[1200];
        double temp1[1200];
        double temp2[1200];

        ifp = fopen("ctsa-master/data/seriesA.txt", "r");
        i = 0;
        if (!ifp) {
            printf("Cannot Open File");
            exit(100);
        }
        while (!feof(ifp)) {
            fscanf(ifp, "%lf\n", &temp[i]);
            i++;
        }
        N = i;

        inp = (double*)malloc(sizeof(double)* N);


        for (i = 0; i < N; ++i) {
            inp[i] = temp[i];
        
        }

    
        obj = auto_arima_init(order,seasonal,s,r,N);

        auto_arima_setApproximation(obj,0);
        auto_arima_setStepwise(obj,0);

        auto_arima_exec(obj,inp,NULL);

        auto_arima_summary(obj);

        auto_arima_predict(obj, inp, NULL, L,NULL, xpred, amse);

        printf("\n");
        printf("Predicted Values : ");
        for (i = 0; i < L; ++i) {
            printf("%g ", xpred[i]);
        }
        printf("\n");
        printf("Standard Errors  : ");
        for (i = 0; i < L; ++i) {
            printf("%g ", sqrt(amse[i]));
        }
        printf("\n");

        auto_arima_free(obj);

        
        free(inp);
        free(xpred);
        free(amse);
        fclose(ifp);
    }

    // Do a time-series forecast with ctsa's autoarima functionailty
    // Expects a DataFrame with 1 column for typing reasons
    // DataList <- DataList <- int
    auto autoARIMA =  [](int futDays) {
        auto retFunc =  [futDays](DataFrame& df) {
            DataList dl = df + adsl::getFirst;
            int i, N, d, D, L;
            double *inp;
            int p, q, P, Q, s, r;
            double *xpred, *amse;
            auto_arima_object obj;
            /*
            Make sure all the parameter values are correct and consistent with other values. eg., if xreg is NULL r should be 0
            or if P = D = Q = 0 then make sure that s is also 0. 
            Recheck the values if the program fails to execute.
            */
            p = 5;
            d = 2;
            q = 5;
            P = 2;
            D = 1;
            Q = 2;
            
            int order[3] = {p,d,q};
            int seasonal[3] = {P,D,Q};

            s = 0;
            r = 0;
            L = futDays;

            xpred = (double*)malloc(sizeof(double)* L);
            amse = (double*)malloc(sizeof(double)* L);

            // Convert input to C-style array
            N = dl.vals.size();
            inp = (double*)malloc(sizeof(double)* N);
            for (i = 0; i < N; ++i) {
                inp[i] = dl.vals[i];
            }
        
            obj = auto_arima_init(order,seasonal,s,r,N);

            auto_arima_setApproximation(obj,0);
            auto_arima_setStepwise(obj,0);

            auto_arima_exec(obj,inp,NULL);

            auto_arima_summary(obj);

            auto_arima_predict(obj, inp, NULL, L,NULL, xpred, amse);

            printf("\n");
            printf("Predicted Values : ");
            for (i = 0; i < L; ++i) {
                printf("%g ", xpred[i]);
            }
            printf("\n");
            printf("Standard Errors  : ");
            for (i = 0; i < L; ++i) {
                printf("%g ", sqrt(amse[i]));
            }
            printf("\n");

            // Convert the predictions and errors to a DataFrame and return
            std::vector<double> predVals(xpred, xpred + L);
            std::vector<double> errorVals(amse, amse + L);
            DataList predList(predVals, "Predicted Value");
            DataList errorList(errorVals, "AMSE Error");
            DataFrame ret;
            ret.addCol(predList);
            ret.addCol(errorList);

            auto_arima_free(obj);
            free(inp);
            free(xpred);
            free(amse);

            return ret;
        };
        return retFunc;
    };
}