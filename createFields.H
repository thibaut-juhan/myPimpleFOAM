Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info << "Reading field s\n" << endl;
volScalarField s
(
   IOobject
   (
      "s",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE      
   ),
   mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<<"Reading field maxDifference\n"<<endl;
volScalarField maxDifference
(
   IOobject
   (
	"maxDifference",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
	IOobject::AUTO_WRITE
   ), 
   mesh
);


Info<< "Reading transportProperties\n" << endl;
IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);
Info<< "Reading diffusivity DT\n" << endl;
dimensionedScalar DT ("DT",dimViscosity, transportProperties);

Info<< "Reading geometryProperties (radius of injectors) \n" << endl;
IOdictionary geometryProperties
(
    IOobject
    (
    	"geometryProperties",
	 runTime.constant(),
	 mesh,
	 IOobject::MUST_READ_IF_MODIFIED,
	 IOobject::NO_WRITE   
    )
);
Info<<"Reading R\n" << endl;
dimensionedScalar R ("R",dimLength,geometryProperties);

#include "createPhi.H"
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, pimple.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());

singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

#include "createMRF.H"
#include "createFvOptions.H"
