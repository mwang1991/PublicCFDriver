/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2017 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The breaking dam free surface problem. This code demonstrates the basic usage of the
 * free surface module in Palabos. Surface tension and contact angles are optional. 
 */

#include "palabos3D.h"
#include "palabos3D.hh"
#include "iCFDriver.h"

using namespace plb;

#define DESCRIPTOR descriptors::ForcedD3Q19Descriptor

typedef double T;


// Smagorinsky constant for LES model.
const T cSmago = 0.14;

// Physical dimensions of the system (in meters).
const T lx = 3.22;
const T ly = 1.0;
const T lz = 1.0;

const T rhoEmpty = T(1);
    
plint writeImagesIter   = 10;
plint getStatisticsIter = 20;

plint maxIter = 10000;
plint N = 40;
T delta_t = 0.001, delta_x = lz / N;
plint nx = util::roundToInt(lx / delta_x);
plint ny = util::roundToInt(ly / delta_x);
plint nz = util::roundToInt(lz / delta_x);
SparseBlockStructure3D blockStructure(createRegularDistribution3D(nx, ny, nz)); //Define space

int ***userflags;

T gLB = 9.8 * delta_t * delta_t/delta_x;
T nuPhys = 0.00001, nuLB, tau = (nuPhys*DESCRIPTOR<T>::invCs2*delta_t)/(delta_x*delta_x) + 0.5, omega = 1./tau, Bo = 100, surfaceTensionLB = rhoEmpty * gLB * N * N / Bo, contactAngle = 80.0;

Array<T,3> externalForce = Array<T,3>(0., 0., -gLB);

std::string outDir;
plint obstacleCenterXYplane, obstacleLength, obstacleWidth, obstacleHeight, beginWaterReservoir, waterReservoirHeight;
//plint waterLevelOne, waterLevelTwo, waterLevelThree, waterLevelFour;

 Dynamics<T,DESCRIPTOR>* dynamics
        = new SmagorinskyBGKdynamics<T,DESCRIPTOR>(omega, cSmago);

MultiScalarField3D<int> *fi;
MultiScalarField3D<double> *fi1;

void setupParameters() {

    // Gravity in lattice units.
    //tau            = (nuPhys*DESCRIPTOR<T>::invCs2*delta_t)/(delta_x*delta_x) + 0.5;
    //omega          = 1./tau;    
    nuLB           = (tau-0.5)*DESCRIPTOR<T>::cs2; // Viscosity in lattice units.
    

    obstacleCenterXYplane = util::roundToInt(0.744*N);
    obstacleLength        = util::roundToInt(0.403*N);
    obstacleWidth         = util::roundToInt(0.161*N);
    pcout<<util::roundToInt(0.403*N)<<std::endl;
    obstacleHeight        = util::roundToInt(0.161*N);
    beginWaterReservoir   = util::roundToInt((0.744+1.248)*N); //water begin position
    waterReservoirHeight  = util::roundToInt(0.55*N);
    
    //waterLevelOne   = util::roundToInt(0.496*N);
    //waterLevelTwo   = util::roundToInt(2.*0.496*N);
    //waterLevelThree = util::roundToInt(3.*0.496*N);
    //waterLevelFour  = util::roundToInt((3.*0.496 + 1.150)*N);
}


int initialUserFluidFlags(plint iX, plint iY, plint iZ) {
	return userflags[iX][iY][iZ];
}


// Specifies the initial condition for the fluid (each cell is assigned the
// flag "fluid", "empty", or "wall").
int initialFluidFlags(plint iX, plint iY, plint iZ) {
    // Place an obstacle on the left end, which is hit by the fluid.
    //pcout<<"iX"<<iX<<"iY"<<iY<<"iZ"<<iZ;


    bool insideObstacle =
        iX >= obstacleCenterXYplane-obstacleWidth/2 &&
        iX <= obstacleCenterXYplane+obstacleWidth/2 &&
        iY >= ny/2-obstacleLength/2 &&
        iY <= ny/2+obstacleLength/2 &&
        iZ <= obstacleHeight+1;
    
    if (insideObstacle) {
        return freeSurfaceFlag::wall;
    }
    else if (iX >= beginWaterReservoir && iZ <= waterReservoirHeight) {
        return freeSurfaceFlag::fluid;
    }
    else {
        return freeSurfaceFlag::empty;
    }
}

int pri(plint iX, plint iY, plint iZ){
	return fi->get(iX,iY,iZ);

}

int pri1(plint iX, plint iY, plint iZ){
	//pcout<<fi1->get(iX,iY,iZ)<<std::endl;
	if(iX == 50 && iY > 20 && iY < 25 && iZ == 20){
		pcout<<"modify"<<std::endl;
		pcout<<fi1->get(iX,iY,iZ)<<std::endl;
		return 1.0;
	}
	return fi1->get(iX,iY,iZ);

}

void writeResults(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, MultiScalarField3D<T>& volumeFraction, plint iT)
{
    static const plint nx = lattice.getNx();
    static const plint ny = lattice.getNy();
    static const plint nz = lattice.getNz();
    Box3D slice(0, nx-1, ny/2, ny/2, 0, nz-1);
    
    // Use a marching-cube algorithm to reconstruct the free surface and write an STL file.
    std::vector<T> isoLevels;
    isoLevels.push_back((T) 0.5);
    typedef TriangleSet<T>::Triangle Triangle;
    std::vector<Triangle> triangles;
    isoSurfaceMarchingCube(triangles, volumeFraction, isoLevels, volumeFraction.getBoundingBox());
    TriangleSet<T>(triangles).writeBinarySTL(createFileName(outDir+"/interface", iT, 6)+".stl");

    //VtkImageOutput3D<T> vtkOut(createFileName("volumeFraction", iT, 6), 1.);
    //vtkOut.writeData<float>(volumeFraction, "vf", 1.);
}

void writeStatistics(FreeSurfaceFields3D<T,DESCRIPTOR>& fields) {
    pcout << " -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- " << std::endl;
    T averageMass = freeSurfaceAverageMass<T,DESCRIPTOR>(fields.freeSurfaceArgs, fields.lattice.getBoundingBox());
    pcout << "Average Mass: " << averageMass  << std::endl;
    T averageDensity = freeSurfaceAverageDensity<T,DESCRIPTOR>(fields.freeSurfaceArgs, fields.lattice.getBoundingBox());
    pcout << "Average Density: " << std::setprecision(12) << averageDensity  << std::endl;

    T averageVolumeFraction = freeSurfaceAverageVolumeFraction<T,DESCRIPTOR>(fields.freeSurfaceArgs, fields.lattice.getBoundingBox());
    pcout << "Average Volume-Fraction: " << std::setprecision(12) << averageVolumeFraction  << std::endl;

    pcout << " -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- " << std::endl;
}


int main(int argc, char **argv)
{
    bool run = false;
    InitServer(5099);


    plbInit(&argc, &argv);
    global::directories().setInputDir("./");
        
    if (global::argc() != 8) {
        pcout << "Error missing some input parameter\n";
    }

    try {
        global::argv(1).read(outDir);
        global::directories().setOutputDir(outDir+"/");

    }
    catch(PlbIOException& except) {
        pcout << except.what() << std::endl;
        pcout << "The parameters for this program are :\n";
        pcout << "1. Output directory name.\n";
        pcout << "2. kinematic viscosity in physical Units (m^2/s) .\n";
        pcout << "3. Bond number (Bo = rho * g * L^2 / gamma).\n";
        pcout << "4. Contact angle (in degrees).\n";
        pcout << "5. number of lattice nodes for lz .\n";
        pcout << "6. delta_t .\n"; 
        pcout << "7. maxIter .\n";
        pcout << "Reasonable parameters on a desktop computer are: " << (std::string)global::argv(0) << " tmp 1.e-5 100 80.0 40 1.e-3 80000\n";
        pcout << "Reasonable parameters on a parallel machine are: " << (std::string)global::argv(0) << " tmp 1.e-6 100 80.0 100 1.e-4 80000\n";
        exit (EXIT_FAILURE);
    }
    
    setupParameters();

    userflags = new int**[nx];
    for(int i = 0; i < nx; i++){
        userflags[i] = new int*[ny];
        for(int j = 0; j < ny; j++){
            userflags[i][j] = new int[nz];
            for(int k = 0; k < nz; k++){
                userflags[i][j][k] = 1;
                //std::cout<<userflags[i][j][k]<<std::endl;
            }
        }
    }
    
    pcout << "delta_t= " << delta_t << std::endl;
    pcout << "delta_x= " << delta_x << std::endl;
    pcout << "delta_t*delta_t/delta_x= " << delta_t*delta_t/delta_x << std::endl;
    pcout << "externalForce= " << externalForce[2] << std::endl;
    pcout << "relaxation time= " << tau << std::endl;
    pcout << "omega= " << omega << std::endl;
    pcout << "kinematic viscosity physical units = " << nuPhys << std::endl;
    pcout << "kinematic viscosity lattice units= " << nuLB << std::endl;
    
    global::timer("initialization").start();
    
	 FreeSurfaceFields3D<T,DESCRIPTOR> fields( blockStructure, dynamics->clone(), rhoEmpty,
	                                      surfaceTensionLB, contactAngle, externalForce );

	 fi = &(fields.flag);
	 fi1 = &(fields.volumeFraction);

    // If surfaceTensionLB is 0, then the surface tension algorithm is deactivated.
    // If contactAngle is less than 0, then the contact angle algorithm is deactivated.
   
    //integrateProcessingFunctional(new ShortenBounceBack3D<T,DESCRIPTOR>, fields.lattice.getBoundingBox(), fields.freeSurfaceArgs, 0);

    // Set all outer-wall cells to "wall" (here, bulk-cells are also set to "wall", but it
    // doesn't matter, because they are overwritten on the next line).
    setToConstant(fields.flag, fields.flag.getBoundingBox(), (int)freeSurfaceFlag::wall);
    // In the bulk (all except outer wall layer), initialize the flags as specified by
    // the function "initialFluidFlags".
    setToFunction(fields.flag, fields.flag.getBoundingBox().enlarge(-1), initialFluidFlags);
    
    fields.defaultInitialize();

    pcout << "Time spent for setting up lattices: "
          << global::timer("initialization").stop() << std::endl;
    T lastIterationTime = T();
    plint iT = 0;
    while(1) {
        
        char command;
        RecvCommand(command);
        std::cout<<command<<std::endl;

        switch(command)
        {
            case '0':
                run = false;
                fields.defaultInitialize();
                iT = 0;
                break;
            case '1':
                run = true;
                break;
            case '2':
                return 0;
        }
        if (run){
            global::timer("iteration").restart();
            T sum_of_mass_matrix = T();
            T lost_mass = T();
            if (iT % getStatisticsIter==0) {
                pcout << std::endl;
                pcout << "ITERATION = " << iT << std::endl;
                pcout << "Time of last iteration is " << lastIterationTime << " seconds" << std::endl;
                //writeStatistics(fields);
                sum_of_mass_matrix = fields.lattice.getInternalStatistics().getSum(0);
                pcout << "Sum of mass matrix: " << sum_of_mass_matrix << std::endl;
                lost_mass = fields.lattice.getInternalStatistics().getSum(1);
                pcout << "Lost mass: " << lost_mass << std::endl;
                pcout << "Total mass: " << sum_of_mass_matrix + lost_mass << std::endl;
                pcout << "Interface cells: " << fields.lattice.getInternalStatistics().getIntSum(0) << std::endl;
            }

            if (iT % writeImagesIter == 0) {
                global::timer("images").start();
                writeResults(fields.lattice, fields.volumeFraction, iT);
                pcout << "Total time spent for writing images: "
                    << global::timer("images").stop() << std::endl;
            } 
            //pcout<<fields.flag.get()
            /*if (iT  == 100) {
            	//setToFunction(fields.volumeFraction, fields.volumeFraction.getBoundingBox().enlarge(-1),pri1);
            	//pcout<<fields.lattice.cells<<std::endl;
       	 		setToFunction(fields.flag, fields.flag.getBoundingBox().enlarge(-1), initialUserFluidFlags);
            	fields.partiallyDefaultInitialize();
            	pcout<<"reset"<<std::endl;
            }*/

            // This includes the collision-streaming cycle, plus all free-surface operations.
            fields.lattice.executeInternalProcessors();
            fields.lattice.evaluateStatistics();
            fields.lattice.incrementTime();

            lastIterationTime = global::timer("iteration").stop();
            iT++;
        }
    }

    for(int i = 0; i < nx; i++){
    for(int j = 0; j < ny; j++)
        delete[] userflags[i][j];
    delete[] userflags[i];
    }
    delete[] userflags;
        
}


