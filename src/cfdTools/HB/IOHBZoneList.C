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

#include "IOHBZoneList.H"

#include "fvMesh.H"
#include "Time.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

Foam::IOobject Foam::IOHBZoneList::createIOobject
(
    const fvMesh& mesh
) const
{
    IOobject io
    (
        "HBProperties",
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );

    if (io.typeHeaderOk<IOdictionary>(true))
    {
        Info<< "Creating HB zone list from " << io.name() << endl;

        io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
        return io;
    }
    else
    {
        Info<< "No HB models present" << nl << endl;

        io.readOpt() = IOobject::NO_READ;
        return io;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::IOHBZoneList::IOHBZoneList
(
    const fvMesh& mesh
)
:
    IOdictionary(createIOobject(mesh)),
    HBZoneList(mesh, *this)
{}


bool Foam::IOHBZoneList::read()
{
    if (regIOobject::read())
    {
        HBZoneList::read(*this);
        return true;
    }

    return false;
}


// ************************************************************************* //
