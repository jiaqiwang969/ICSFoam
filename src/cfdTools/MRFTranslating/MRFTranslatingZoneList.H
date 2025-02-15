/*---------------------------------------------------------------------------*\

    ICSFoam: a library for Implicit Coupled Simulations in OpenFOAM
  
    Copyright (C) 2022  Stefano Oliani

    https://turbofe.it

-------------------------------------------------------------------------------
License
    This file is part of ICSFOAM.

    ICSFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICSFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ICSFOAM.  If not, see <http://www.gnu.org/licenses/>.


Author
    Stefano Oliani
    Fluid Machinery Research Group, University of Ferrara, Italy
\*---------------------------------------------------------------------------*/

#ifndef MRFTranslatingZoneList_H
#define MRFTranslatingZoneList_H

#include "fvMesh.H"
#include "dictionary.H"
#include "fvMatricesFwd.H"
#include "MRFTranslatingZone.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators
class MRFTranslatingZoneList;
Ostream& operator<<(Ostream& os, const MRFTranslatingZoneList& models);

/*---------------------------------------------------------------------------*\
                         Class MRFTranslatingZoneList Declaration
\*---------------------------------------------------------------------------*/

class MRFTranslatingZoneList
:
    public PtrList<MRFTranslatingZone>
{
    // Private Member Functions

        //- No copy construct
        MRFTranslatingZoneList(const MRFTranslatingZoneList&) = delete;

        //- No copy assignment
        void operator=(const MRFTranslatingZoneList&) = delete;


protected:

    // Protected data

        //- Reference to the mesh database
        const fvMesh& mesh_;


public:

    //- Constructor
    MRFTranslatingZoneList(const fvMesh& mesh, const dictionary& dict);

    //- Destructor
    ~MRFTranslatingZoneList();


    // Member Functions

		//- Return origin over all zones
		tmp<volVectorField> frameVel() const;

		//- Return pseudo mesh flux
	   tmp<surfaceVectorField> faceU() const;

        //- Return active status
        bool active(const bool warn = false) const;

        //- Reset the source list
        void reset(const dictionary& dict);

        //- Make the given absolute velocity relative within the MRF region
        void makeRelative(volVectorField& U) const;

        //- Make the given absolute flux relative within the MRF region
        void makeRelative(surfaceScalarField& phi) const;

        //- Return the given absolute flux relative within the MRF region
        tmp<surfaceScalarField> relative
        (
            const tmp<surfaceScalarField>& phi
        ) const;

        //- Return the given absolute boundary flux relative within
        //  the MRF region
        tmp<FieldField<fvsPatchField, scalar>> relative
        (
            const tmp<FieldField<fvsPatchField, scalar>>& tphi
        ) const;

        //- Return the given absolute patch flux relative within
        //  the MRF region
        tmp<Field<scalar>> relative
        (
            const tmp<Field<scalar>>& tphi,
            const label patchi
        ) const;

        //- Make the given absolute mass-flux relative within the MRF region
        void makeRelative
        (
            const surfaceScalarField& rho,
            surfaceScalarField& phi
        ) const;

        //- Make the given relative velocity absolute within the MRF region
        void makeAbsolute(volVectorField& U) const;

        //- Make the given relative flux absolute within the MRF region
        void makeAbsolute(surfaceScalarField& phi) const;

        //- Return the given relative flux absolute within the MRF region
        tmp<surfaceScalarField> absolute
        (
            const tmp<surfaceScalarField>& phi
        ) const;

        //- Make the given relative mass-flux absolute within the MRF region
        void makeAbsolute
        (
            const surfaceScalarField& rho,
            surfaceScalarField& phi
        ) const;

        //- Correct the boundary velocity for the rotation of the MRF region
        void correctBoundaryVelocity(volVectorField& U) const;

        //- Correct the boundary flux for the rotation of the MRF region
        void correctBoundaryFlux
        (
            const volVectorField& U,
            surfaceScalarField& phi
        ) const;

        //- Filter-out the MRF region contribution from the given field
        // setting the corresponding values to zero
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> zeroFilter
        (
            const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>& tphi
        ) const;

        //- Update MRFTranslatingZone faces if the mesh topology changes
        void update();


        // I-O

            //- Read dictionary
            bool read(const dictionary& dict);

            //- Write data to Ostream
            bool writeData(Ostream& os) const;

            //- Ostream operator
            friend Ostream& operator<<
            (
                Ostream& os,
                const MRFTranslatingZoneList& models
            );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "MRFTranslatingZoneListTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
