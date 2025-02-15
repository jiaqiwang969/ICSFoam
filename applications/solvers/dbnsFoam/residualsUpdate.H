// Construct residuals of eqn system: [A] \Delta W = [R]
volScalarField rhoR =
(
    -fvc::div(phi)
);

volVectorField rhoUR =
(
    -fvc::div(phiUp)
);

volScalarField rhoER =
(
    -fvc::div(phiEp)
);

// Viscous flow
if (!inviscid)
{
    volScalarField muEff("muEff", turbulence->muEff());

    volScalarField alphaEff("alphaEff", turbulence->alphaEff());

    volTensorField& tauMC = pTauMC();

    tauMC = muEff*dev2(Foam::T(fvc::grad(U)));
    rhoUR += fvc::laplacian(muEff,U);
    rhoUR += fvc::div(tauMC);

    surfaceVectorField sigmaDotU
    (
        "sigmaDotU",
        (
          fvc::interpolate(muEff)*fvc::interpolate(fvc::grad(U))
          + fvc::interpolate(tauMC)
        )
        & fvc::interpolate(U)
    );

    rhoER += fvc::div(sigmaDotU & mesh.Sf());

    volScalarField eCalc("eCalc", rhoE/rho-0.5*magSqr(U)); 

    rhoER += fvc::laplacian(alphaEff, eCalc, "laplacian(alphaEff,e)");
}

// Pseudo and real-time contribution

tmp<fvScalarMatrix> ddtRho = fvm::ddt(rho);
tmp<fvVectorMatrix> ddtRhoU = fvm::ddt(rhoU);
tmp<fvScalarMatrix> ddtRhoE = fvm::ddt(rhoE);

// Work around bug causing timeIndex not to be stored for ddt0 field (CrankNicolson)
// Seems to be caused by commented out line in GeometricField::storeOldTimes(),
if (mesh.foundObject<volScalarField>("ddt0(rho)"))
{
    const_cast<volScalarField&>
    (
        mesh.lookupObject<volScalarField>("ddt0(rho)")
    ).timeIndex() = mesh.time().timeIndex();
}

if (mesh.foundObject<volVectorField>("ddt0(rhoU)"))
{
    const_cast<volVectorField&>
    (
        mesh.lookupObject<volVectorField>("ddt0(rhoU)")
    ).timeIndex() = mesh.time().timeIndex();
}

if (mesh.foundObject<volScalarField>("ddt0(rhoE)"))
{
    const_cast<volScalarField&>
    (
        mesh.lookupObject<volScalarField>("ddt0(rhoE)")
    ).timeIndex() = mesh.time().timeIndex();
}

if (!steadyState)
{
    // NOTE: Solving the system A \Delta W^\tau = B and therefore the
    // source term needs to be adjusted as below.
    // For steady state analysis this term reduces it to zero.
    rhoR.primitiveFieldRef() -= (ddtRho().diag()*rho-ddtRho().source())/mesh.V();
    rhoUR.primitiveFieldRef() -= (ddtRhoU().diag()*rhoU-ddtRhoU().source())/mesh.V();
    rhoER.primitiveFieldRef() -= (ddtRhoE().diag()*rhoE-ddtRhoE().source())/mesh.V();
}

// NOTE: These should be equal if ddtScheme is consistent
ddtCoeff = max(max(ddtRho().diag(),ddtRhoU().diag()),ddtRhoE().diag())/mesh.V();

eqSystem.dSByS(0,0).source() = rhoR.primitiveField()*mesh.V();
eqSystem.dVByV(0,0).source() = rhoUR.primitiveField()*mesh.V();
eqSystem.dSByS(1,1).source() = rhoER.primitiveField()*mesh.V();

