#pragma once
// Header for calling to Dlib for machine learning

#include "DataFrame.h"
#include "Ops.h"
#include "DlibConst.h"
#include <dlib/svm.h>

namespace adsl {

	// Run http://dlib.net/svm_c_ex.cpp.html
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

            // Split randomized data into testing and training
            // TODO: look into removing source after copying to dest one-by-one
            int cutoff = (int)(samples.size() * 0.7); // 70-30
            std::vector<sample_type> samplesTrain, samplesTest;
            std::vector<double> labelsTrain, labelsTest;
            for (int i = 0; i < samples.size(); i++) {
                if (i < cutoff) {
                    samplesTrain.push_back(samples[i]);
                    labelsTrain.push_back(labels[i]);
                }
                else {
                    samplesTest.push_back(samples[i]);
                    labelsTest.push_back(labels[i]);
                }
            }

            // find optimal c and gamma values
            // looks like anything will work, so keep orig from tutorial
            svm_c_trainer<kernel_type> trainer;
            cout << "doing cross validation" << endl;
            for (double gamma = 0.00001; gamma <= 1; gamma *= 5)
            {
                for (double C = 1; C < 100000; C *= 5)
                {
                    trainer.set_kernel(kernel_type(gamma));
                    trainer.set_c(C);
                    cout << "gamma: " << gamma << "    C: " << C;
                    cout << "     cross validation accuracy: "
                        << cross_validate_trainer(trainer, samplesTrain, labelsTrain, 10);
                }
            }

            trainer.set_kernel(kernel_type(0.5));
            trainer.set_c(10000);
            typedef decision_function<kernel_type> dec_funct_type;
            typedef normalized_function<dec_funct_type> funct_type;

            funct_type learned_function;
            learned_function.normalizer = normalizer;  // save normalization information
            learned_function.function = trainer.train(samplesTrain, labelsTrain); // perform the actual SVM training and save the results
            
            cout << "\nnumber of support vectors in our learned_function is "
                << learned_function.function.basis_vectors.size() << endl;
            
            // Test the model
            int correct = 0;
            for (int i = 0; i < samplesTest.size(); i++) {
                cout << "Observed: " << labelsTest[i] << " Predicted " << learned_function(samplesTest[i]) << endl;
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
            cout << "Model accuracy: " << ((double)correct / (double)samplesTest.size()) * 100.0 << endl;

            DataFrame ret;
            return ret;
        }; 
        return retFunc;
    };
}