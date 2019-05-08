/*
 * =====================================================================================
 *
 *       Filename:  tasep.cpp
 *
 *    Description:  Totally asymmetric simple exclusion process
 *
 *        Version:  1.0
 *        Created:  07/05/2019
 *       Revision:  none
 *
 *         Author:  Sachin Krishnan T V
 *   Organization:  Indian Institute of Technology Madras
 *
 * =====================================================================================
 */
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <random>
#include <png.h>
#include "../pcg_random/pcg_random.hpp"

#define OUTPUT_FREQUENCY 1000

using namespace std;

vector < vector <int> > latEvolution;

// Write the  output to PNG file
void outputToPng(int timeSteps, int latSize)
{
    int x,y;
    int width = latSize, height = timeSteps;
    png_byte bit_depth = 8;

    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;

    char filename[] = "config.png";

    FILE *dumpf = fopen(filename, "wb");
    if(!dumpf)
        cout<<"[outputToPng] File could not be opened for writing."<<endl;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
        cout<<"[outputToPng] png_create_write_struct failed."<<endl;

    info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
        cout<<"[outputToPng] png_create_info_struct failed."<<endl;

    if(setjmp(png_jmpbuf(png_ptr)))
        cout<<"[outputToPng] Error during init_io."<<endl;

    png_init_io(png_ptr, dumpf);

    if(setjmp(png_jmpbuf(png_ptr)))
        cout<<"[outputToPng] Error during writing header."<<endl;

    png_set_IHDR(png_ptr, info_ptr, width, height,
            bit_depth, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    if(setjmp(png_jmpbuf(png_ptr)))
        cout<<"[outputToPng] Error while writing bytes."<<endl;

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for(y = 0;y < height;y++)
    {
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
        for(x = 0; x < width;x++)
        {
            if(latEvolution[y][x] == 1)
            {
                row_pointers[y][x*4] = 0;
                row_pointers[y][x*4+1] = 0;
                row_pointers[y][x*4+2] = 0;
                row_pointers[y][x*4+3] = 255;
            }
            else
            {
                row_pointers[y][x*4] = 255;
                row_pointers[y][x*4+1] = 255;
                row_pointers[y][x*4+2] = 255;
                row_pointers[y][x*4+3] = 255;
            }
        }
    }

    png_write_image(png_ptr, row_pointers);

    if(setjmp(png_jmpbuf(png_ptr)))
        cout<<"[outputToPng] Error during end of write."<<endl;

    png_write_end(png_ptr, NULL);

    for(y = 0;y < height;y++)
        free(row_pointers[y]);
    free(row_pointers);

    png_destroy_write_struct(&png_ptr,&info_ptr);

    fclose(dumpf);
}

int main(int argc, char *argv[])
{
    int i, j;
    int mcMove;
    int ranNum;
    int outputTimestep;
    long int mcSweep;
    long int totalTime;
    vector <int> lattice;
    int latSize;
    double alpha;
    double beta;
    double gamma;
    double particleCurrent;

    if(argc < 6)
    {
        cout<<"Usage: "<<argv[0]<<" <lattice-size> <alpha> <beta> <gamma> <simulation-time>"<<endl;
        return 0;
    }
    latSize = atoi(argv[1]);
    alpha = atof(argv[2]);
    beta = atof(argv[3]);
    gamma = atof(argv[4]);
    totalTime = atol(argv[5]);

    pcg_extras::seed_seq_from<random_device> seed_source;
    pcg32 rng(seed_source);
    uniform_real_distribution<double> udist(0, 1);

    // initialize empty lattice
    for(i = 0;i < latSize;i++)
        lattice.push_back(0);

    particleCurrent = 0.0;
    outputTimestep = 0;
    for(i = 0;i < totalTime/OUTPUT_FREQUENCY;i++)
    {
        latEvolution.push_back(vector <int>());
        for(j = 0; j < latSize;j++)
            latEvolution[i].push_back(0);
    }
    for(mcSweep = 1;mcSweep < totalTime;mcSweep++)
    {
        // Particle entry
        if(lattice[0] == 0)
            if(udist(rng) < alpha)
                lattice[0] = 1;

        // Particle hopping
        for(mcMove = 1;mcMove < latSize;mcMove++)
        {
            ranNum = (int) (udist(rng) * latSize);
            if(ranNum == latSize - 1) continue;
            if(lattice[ranNum] == 1 && lattice[ranNum + 1] == 0)
            {
                if(udist(rng) < gamma)
                {
                    lattice[ranNum + 1] = 1;
                    lattice[ranNum] = 0;
                }
            }
        }

        // Particle exit
        if(lattice[latSize - 1] == 1)
            if(udist(rng) < beta)
            {
                lattice[latSize - 1] = 0;
                particleCurrent+=1.0;
            }

        if(mcSweep % OUTPUT_FREQUENCY == 0)
        {
            for(i = 0;i < latSize;i++)
            {
                latEvolution[outputTimestep][i] = lattice[i];
            }
            outputTimestep++;
        }
    }


    outputToPng(outputTimestep,latSize);

    cout<<"Particle current = "<<particleCurrent/totalTime<<endl;
    // Cleanup
    lattice.clear();
    latEvolution.clear();

    return 0;
}
