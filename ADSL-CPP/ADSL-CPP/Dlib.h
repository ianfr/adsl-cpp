#pragma once
// Header for calling to Dlib for machine learning

#include "DataFrame.h"
#include "Ops.h"
#include "DlibConst.h"
#include "GSL.h"

#include <random>
#include <dlib/svm.h>

namespace adsl {

	// Run a copy of http://dlib.net/svm_c_ex.cpp.html to test that Dlib is working
	void dlib_run_svm_c_trainer() {
		using namespace std;
		using namespace dlib;

        typedef matrix<double, 2, 1> sample_type;

        typedef radial_basis_kernel<sample_type> kernel_type;

        // Now we make objects to contain our samples and their respective labels.
        std::vector<sample_type> samples;
        std::vector<double> labels;

        // Now let's put some data into our samples and labels objects.  We do this
        // by looping over a bunch of points and labeling them according to their
        // distance from the origin.
        for (int r = -20; r <= 20; ++r)
        {
            for (int c = -20; c <= 20; ++c)
            {
                sample_type samp;
                samp(0) = r;
                samp(1) = c;
                samples.push_back(samp);

                // if this point is less than 10 from the origin
                if (sqrt((double)r * r + c * c) <= 10)
                    labels.push_back(+1);
                else
                    labels.push_back(-1);

            }
        }

        vector_normalizer<sample_type> normalizer;
        // Let the normalizer learn the mean and standard deviation of the samples.
        normalizer.train(samples);
        // now normalize each sample
        for (unsigned long i = 0; i < samples.size(); ++i)
            samples[i] = normalizer(samples[i]);

        randomize_samples(samples, labels);

        // here we make an instance of the svm_c_trainer object that uses our kernel
        // type.
        svm_c_trainer<kernel_type> trainer;

        // Now we loop over some different C and gamma values to see how good they
        // are.  
        cout << "doing cross validation" << endl;
        for (double gamma = 0.00001; gamma <= 1; gamma *= 5)
        {
            for (double C = 1; C < 100000; C *= 5)
            {
                // tell the trainer the parameters we want to use
                trainer.set_kernel(kernel_type(gamma));
                trainer.set_c(C);

                cout << "gamma: " << gamma << "    C: " << C;
                cout << "     cross validation accuracy: "
                    << cross_validate_trainer(trainer, samples, labels, 3);
            }
        }

        trainer.set_kernel(kernel_type(0.15625));
        trainer.set_c(5);
        typedef decision_function<kernel_type> dec_funct_type;
        typedef normalized_function<dec_funct_type> funct_type;
  
        funct_type learned_function;
        learned_function.normalizer = normalizer;  // save normalization information
        learned_function.function = trainer.train(samples, labels); // perform the actual SVM training and save the results

        // print out the number of support vectors in the resulting decision function
        cout << "\nnumber of support vectors in our learned_function is "
            << learned_function.function.basis_vectors.size() << endl;

        // Now let's try this decision_function on some samples we haven't seen before.
        sample_type sample;

        sample(0) = 3.123;
        sample(1) = 2;
        cout << "This is a +1 class example, the classifier output is " << learned_function(sample) << endl;

        sample(0) = 3.123;
        sample(1) = 9.3545;
        cout << "This is a +1 class example, the classifier output is " << learned_function(sample) << endl;

        sample(0) = 13.123;
        sample(1) = 9.3545;
        cout << "This is a -1 class example, the classifier output is " << learned_function(sample) << endl;

        sample(0) = 13.123;
        sample(1) = 0;
        cout << "This is a -1 class example, the classifier output is " << learned_function(sample) << endl;


        // We can also train a decision function that reports a well conditioned
        // probability instead of just a number > 0 for the +1 class and < 0 for the
        // -1 class.  An example of doing that follows:
        typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;
        typedef normalized_function<probabilistic_funct_type> pfunct_type;

        pfunct_type learned_pfunct;
        learned_pfunct.normalizer = normalizer;
        learned_pfunct.function = train_probabilistic_decision_function(trainer, samples, labels, 3);
        // Now we have a function that returns the probability that a given sample is of the +1 class.  

        // print out the number of support vectors in the resulting decision function.  
        // (it should be the same as in the one above)
        cout << "\nnumber of support vectors in our learned_pfunct is "
            << learned_pfunct.function.decision_funct.basis_vectors.size() << endl;

        sample(0) = 3.123;
        sample(1) = 2;
        cout << "This +1 class example should have high probability.  Its probability is: "
            << learned_pfunct(sample) << endl;

        sample(0) = 3.123;
        sample(1) = 9.3545;
        cout << "This +1 class example should have high probability.  Its probability is: "
            << learned_pfunct(sample) << endl;

        sample(0) = 13.123;
        sample(1) = 9.3545;
        cout << "This -1 class example should have low probability.  Its probability is: "
            << learned_pfunct(sample) << endl;

        sample(0) = 13.123;
        sample(1) = 0;
        cout << "This -1 class example should have low probability.  Its probability is: "
            << learned_pfunct(sample) << endl;



        // Another thing that is worth knowing is that just about everything in dlib
        // is serializable.  So for example, you can save the learned_pfunct object
        // to disk and recall it later like so:
        serialize("saved_function.dat") << learned_pfunct;

        // Now let's open that file back up and load the function object it contains.
        deserialize("saved_function.dat") >> learned_pfunct;
       
        cout << "\ncross validation accuracy with only 10 support vectors: "
            << cross_validate_trainer(reduced2(trainer, 10), samples, labels, 3);

        // Let's print out the original cross validation score too for comparison.
        cout << "cross validation accuracy with all the original support vectors: "
            << cross_validate_trainer(trainer, samples, labels, 3);

        // To get the reduced decision function out we would just do this:
        learned_function.function = reduced2(trainer, 10).train(samples, labels);
        // And similarly for the probabilistic_decision_function: 
        learned_pfunct.function = train_probabilistic_decision_function(reduced2(trainer, 10), samples, labels, 3);

	}

    // Train a classifier for predicting a true/false label given:
        // Labeled data, <100 features, and <20K samples
    // A features DataFrame is passed in w/ +
    // The argument is a vector of labels
    auto binaryClassifier = [](std::vector<double>& labels) {
        auto retFunc = [&labels](DataFrame& features) {
            using namespace std;
            using namespace dlib;

            // Start
            typedef matrix<double, MAX_BIN_F, 1> sample_type; // TODO: figure out how to use set_size() instead of 99
            typedef radial_basis_kernel<sample_type> kernel_type;
            std::vector<sample_type> samples;
            
            // Load in data from input DataFrame
            assert(features.getCols() <= MAX_BIN_F);
            for (int rowI = 0; rowI < features.getRows(); rowI++) {
                sample_type samp;
                for (int colI = 0; colI < features.getCols(); colI++) {
                    samp(colI) = features.getData(colI, rowI);
                }
                samples.push_back(samp);
            }

            // Normalize
            vector_normalizer<sample_type> normalizer;
            normalizer.train(samples);
            for (unsigned long i = 0; i < samples.size(); ++i)
                samples[i] = normalizer(samples[i]);

            cout << "labels: ";  for (auto a : labels) cout << a << " "; cout << endl;
            randomize_samples(samples, labels);

            // Split randomized data into training, testing, and validation
            // 50-25-25
            int cutoff0 = (int)(samples.size() * 0.5);
            int cutoff1 = (int)(samples.size() * 0.75);
            std::vector<sample_type> samplesTrain, samplesTest, samplesVal;
            std::vector<double> labelsTrain, labelsTest, labelsVal;
            for (int i = 0; i < samples.size(); i++) {
                if (i < cutoff0) {
                    samplesTrain.push_back(samples[i]);
                    labelsTrain.push_back(labels[i]);
                }
                else if (i < cutoff1) {
                    samplesTest.push_back(samples[i]);
                    labelsTest.push_back(labels[i]);
                }
                else {
                    samplesVal.push_back(samples[i]);
                    labelsVal.push_back(labels[i]);
                }
            }
            
            svm_c_trainer<kernel_type> trainer;
            typedef decision_function<kernel_type> dec_funct_type;
            typedef normalized_function<dec_funct_type> funct_type;
            funct_type learned_function;
            
            // Perform a grid search to find best parameters for the trainer
            cout << "Search for C and gamma based on test accuracy:" << endl;
            double bestAccuracy = 0;
            double bestGamma = 0;
            double bestC = 0;
            for (double gamma = GRID_GAMMA_MIN; gamma <= GRID_GAMMA_MAX; gamma *= 5)
            {
                for (double C = GRID_C_MIN; C < GRID_C_MAX; C *= 5)
                {
                    trainer.set_kernel(kernel_type(gamma));
                    trainer.set_c(C);

                    learned_function.normalizer = normalizer;  // save normalization information
                    learned_function.function = trainer.train(samplesTrain, labelsTrain);

                    int correct = 0;
                    for (int i = 0; i < samplesTest.size(); i++) {
                        if (learned_function(samplesTest[i]) > 0 && labelsTest[i] > 0) {
                            correct += 1;
                        }
                        else if (learned_function(samplesTest[i]) < 0 && labelsTest[i] < 0) {
                            correct += 1;
                        }
                        else {
                            //pass
                        }
                    }
                    double accuracy = ((double)correct / (double)samplesTest.size()) * 100.0;
                    if (accuracy > bestAccuracy) {
                        bestC = C;
                        bestGamma = gamma;
                        bestAccuracy = accuracy;
                    }
                    cout << "gamma: " << gamma << "    C: " << C;
                    cout << "     test accuracy: "
                        << accuracy << endl;
                }
            }

            cout << "Best accuracy before optimizing: " << bestAccuracy << " with gamma " << bestGamma << " and C " << bestC << endl;

            // Run optimizer to return the best C and gamma

            // Optimize the binary classifier with a genetic algorithm
            // Returns a vector {gamma, C}
            // x0 is the initial point
            auto optimizer = [&] (vd x0, int popsize) -> vd {

                // fitness function
                auto fitness = [&] (double gamma, double C) -> double {
                    trainer.set_kernel(kernel_type(gamma));
                    trainer.set_c(C);
                    learned_function.normalizer = normalizer;  // save normalization information
                    learned_function.function = trainer.train(samplesTrain, labelsTrain);

                    int correct = 0;
                    for (int i = 0; i < samplesTest.size(); i++) {
                        if (learned_function(samplesTest[i]) > 0 && labelsTest[i] > 0) {
                            correct += 1;
                        }
                        else if (learned_function(samplesTest[i]) < 0 && labelsTest[i] < 0) {
                            correct += 1;
                        }
                        else {}
                    }
                    double accuracy = ((double)correct / (double)samplesTest.size()) * 100.0;
                    return accuracy;
                };

                DataFrame pop; // population
                pop.init(3); // 3 cols
                pop.setNames({ "gamma", "c", "accuracy" });
                // make sure the initial guess is part of the initial population
                pop.addRow({ x0[0], x0[1], bestAccuracy});

                // populate the population with random variations on the initial point
                double percentage = PERC_START;
                std::random_device seederG;
                std::mt19937 rngG(seederG());
                std::uniform_real_distribution<double> disG(-percentage / 100, percentage / 100);
                std::random_device seederC;
                std::mt19937 rngC(seederC());
                std::uniform_real_distribution<double> disC(-percentage / 100, percentage / 100);
                for (int i = 0; i < popsize-1; i++) {
                    double tmpGamma = x0[0] * (1.0 + disG(rngG));
                    double tmpC = x0[1] * (1.0 + disC(rngC));
                    double tmpAcc = fitness(tmpGamma, tmpC);
                    pop.addRow({ tmpGamma, tmpC, tmpAcc });
                }

                for (int generations = 0; generations < MAX_GENS; generations++) {

                    // check if the desired threshold accuracy has been reached
                    for (int rowInd = 0; rowInd < pop.getRows(); rowInd++) {
                        int colInd = pop.getColIndex("accuracy");
                        if (pop.getData(colInd, rowInd) >= THRESH_ACC) {
                            int gammaInd = pop.getColIndex("gamma");
                            int cInd = pop.getColIndex("c");
                            return { pop.getData(gammaInd, rowInd), pop.getData(cInd, rowInd), pop.getData(colInd, rowInd) };
                        }
                    }

                    // rank the population by their fitness
                    std::vector<int> rankedInds = pop.getSortedIndices("accuracy");

                    // mutate the top 49.999...% of the population **slightly**
                    int halfMarker = (int)(rankedInds.size() / 2);
                    int threeFourthsMarker = (int)((double)rankedInds.size() * 0.75);
                    percentage = PERC_TOP;
                    std::uniform_real_distribution<double> disG_1(-percentage / 100, percentage / 100);
                    std::uniform_real_distribution<double> disC_1(-percentage / 100, percentage / 100);
                    int gammaInd = pop.getColIndex("gamma");
                    int cInd = pop.getColIndex("c");
                    int accInd = pop.getColIndex("accuracy");

                    double avgGammaTopHalf = 0; // keep track of this for later
                    double avgCTopHalf = 0; // keep track of this for later
                    for (int i = 1; i < halfMarker; i++) {
                        avgGammaTopHalf += pop.getData(gammaInd, rankedInds[i]);
                        avgCTopHalf += pop.getData(cInd, rankedInds[i]);
                        double tmpGamma = pop.getData(gammaInd, rankedInds[i]) * (1.0 + disG_1(rngG));
                        double tmpC = pop.getData(cInd, rankedInds[i]) * (1.0 + disC_1(rngC));
                        double tmpAcc = fitness(tmpGamma, tmpC);
                        pop.replaceRow(rankedInds[i], { tmpGamma, tmpC, tmpAcc });
                    }

                    avgGammaTopHalf = avgGammaTopHalf / (double)halfMarker;
                    avgCTopHalf = avgCTopHalf / (double)halfMarker;

                    // replace the next-to-the-bottom 25% of the population with **large** variations on the average of the top 50%
                    percentage = PERC_MIDDLE;
                    std::uniform_real_distribution<double> disG_2(-percentage / 100, percentage / 100);
                    std::uniform_real_distribution<double> disC_2(-percentage / 100, percentage / 100);

                    for (int i = halfMarker; i < threeFourthsMarker; i++) {
                        double tmpGamma = avgGammaTopHalf * (1.0 + disG_2(rngG));
                        double tmpC = avgCTopHalf * (1.0 + disC_2(rngC));
                        double tmpAcc = fitness(tmpGamma, tmpC);
                        pop.replaceRow(rankedInds[i], { tmpGamma, tmpC, tmpAcc });
                    }

                    // replace the bottom 25% of the population with purely random solutions
                    std::uniform_real_distribution<double> disG_3(GA_GAMMA_MIN, GA_GAMMA_MAX);
                    std::uniform_real_distribution<double> disC_3(GA_C_MIN, GA_C_MAX);

                    for (int i = threeFourthsMarker; i < rankedInds.size(); i++) {
                        double tmpGamma = disG_3(rngG);
                        double tmpC = disC_3(rngC);
                        double tmpAcc = fitness(tmpGamma, tmpC);
                        pop.replaceRow(rankedInds[i], { tmpGamma, tmpC, tmpAcc });
                    }

                } // end generations loop

                std::vector<int> rankedInds = pop.getSortedIndices("accuracy");
                return { pop.getData(0, rankedInds[0]), 
                    pop.getData(1, rankedInds[0]),
                    pop.getData(2, rankedInds[0]) };
            };
            
            vd x0 = {bestGamma, bestC};
            vd best = optimizer(x0, POP_SIZE);
            bestGamma = best[0];
            bestC = best[1];
            bestAccuracy = best[2];
            
            cout << "Best accuracy after optimizing: " << bestAccuracy << " with gamma " << bestGamma << " and C " << bestC << endl;

            trainer.set_kernel(kernel_type(bestGamma));
            trainer.set_c(bestC);

            learned_function.normalizer = normalizer;  // save normalization information
            learned_function.function = trainer.train(samplesTrain, labelsTrain); // perform the actual SVM training and save the results
            
            cout << "\nnumber of support vectors in our learned_function is "
                << learned_function.function.basis_vectors.size() << endl;
            
            // Test the model with validation data
            int correct = 0;
            for (int i = 0; i < samplesVal.size(); i++) {
                //cout << "Observed: " << labelsVal[i] << " Predicted " << learned_function(samplesVal[i]) << endl;
                if (learned_function(samplesVal[i]) > 0 && labelsVal[i] > 0) {
                    correct += 1;
                }
                else if (learned_function(samplesVal[i]) < 0 && labelsVal[i] < 0) {
                    correct += 1;
                }
                else {
                    //pass
                }
            }
            cout << "Model validation accuracy: " << ((double)correct / (double)samplesVal.size()) * 100.0 << endl;

            DataFrame ret;
            return ret;
        }; 
        return retFunc;
    };

}