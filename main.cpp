// include usable functions
#include "Stuff.hh"

// main function
// argv[1] is datafile to fit
//      1st col --> some physical quantity (x)
//      2nd col --> data (y)
//      3rd col --> err (sigma)
// rest of cols --> Jackknife samples (y_jck)
// argv[2] is the divisor
int main(int, char **argv)
{
    // file name
    std::string fileName = "None";
    // check for arguments
    fileName = argv[1];
    
    // error check
    if (fileName == "None")
    {
        std::cout << "No file was given, or the file dose not exist, or unavailable." << std::endl;
        std::exit(-1);
    }

    // divisor
    int divisor = std::atoi(argv[2]);

    // read file to matrix
    Eigen::MatrixXd const rawDataMat = ReadFile(fileName);

    // size of raw data
    int rows = rawDataMat.rows(), cols = rawDataMat.cols();

    // jackknife samples (for each measurement)
    std::vector<Eigen::VectorXd> JCKSamples(rows, Eigen::VectorXd(cols - 3));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols - 3; j++)
        {
            JCKSamples[i][j] = rawDataMat(i, j + 3);
        }
    }

    // test for 1st data set
    Eigen::VectorXd JCKReducedSamples = JCKReduced(JCKSamples[0], divisor);


    // write to screen
    std::cout << JCKReducedSamples << std::endl;
    std::cout << "Mean: " << JCKReducedSamples.mean() << std::endl;
    std::cout << "Sigma: " << std::sqrt(variance(JCKReducedSamples, divisor, JCKReducedSamples.mean())) << std::endl;;
}