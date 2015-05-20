#ifndef BOOST_GEOMETRY_PROJECTIONS_OEA_HPP
#define BOOST_GEOMETRY_PROJECTIONS_OEA_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/aasincos.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace oea{

            struct par_oea
            {
                double    theta;
                double    m, n;
                double    two_r_m, two_r_n, rm, rn, hm, hn;
                double    cp0, sp0;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_oea_spheroid : public base_t_fi<base_oea_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_oea m_proj_parm;

                inline base_oea_spheroid(const Parameters& par)
                    : base_t_fi<base_oea_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double Az, M, N, cp, sp, cl, shz;

                    cp = cos(lp_lat);
                    sp = sin(lp_lat);
                    cl = cos(lp_lon);
                    Az = aatan2(cp * sin(lp_lon), this->m_proj_parm.cp0 * sp - this->m_proj_parm.sp0 * cp * cl) + this->m_proj_parm.theta;
                    shz = sin(0.5 * aacos(this->m_proj_parm.sp0 * sp + this->m_proj_parm.cp0 * cp * cl));
                    M = aasin(shz * sin(Az));
                    N = aasin(shz * cos(Az) * cos(M) / cos(M * this->m_proj_parm.two_r_m));
                    xy_y = this->m_proj_parm.n * sin(N * this->m_proj_parm.two_r_n);
                    xy_x = this->m_proj_parm.m * sin(M * this->m_proj_parm.two_r_m) * cos(N) / cos(N * this->m_proj_parm.two_r_n);
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double N, M, xp, yp, z, Az, cz, sz, cAz;

                    N = this->m_proj_parm.hn * aasin(xy_y * this->m_proj_parm.rn);
                    M = this->m_proj_parm.hm * aasin(xy_x * this->m_proj_parm.rm * cos(N * this->m_proj_parm.two_r_n) / cos(N));
                    xp = 2. * sin(M);
                    yp = 2. * sin(N) * cos(M * this->m_proj_parm.two_r_m) / cos(M);
                    cAz = cos(Az = aatan2(xp, yp) - this->m_proj_parm.theta);
                    z = 2. * aasin(0.5 * boost::math::hypot(xp, yp));
                    sz = sin(z);
                    cz = cos(z);
                    lp_lat = aasin(this->m_proj_parm.sp0 * cz + this->m_proj_parm.cp0 * sz * cAz);
                    lp_lon = aatan2(sz * sin(Az),
                        this->m_proj_parm.cp0 * cz - this->m_proj_parm.sp0 * sz * cAz);
                }
            };

            // Oblated Equal Area
            template <typename Parameters>
            void setup_oea(Parameters& par, par_oea& proj_parm)
            {
                if (((proj_parm.n = pj_param(par.params, "dn").f) <= 0.) ||
                    ((proj_parm.m = pj_param(par.params, "dm").f) <= 0.))
                    throw proj_exception(-39);
                else {
                    proj_parm.theta = pj_param(par.params, "rtheta").f;
                    proj_parm.sp0 = sin(par.phi0);
                    proj_parm.cp0 = cos(par.phi0);
                    proj_parm.rn = 1./ proj_parm.n;
                    proj_parm.rm = 1./ proj_parm.m;
                    proj_parm.two_r_n = 2. * proj_parm.rn;
                    proj_parm.two_r_m = 2. * proj_parm.rm;
                    proj_parm.hm = 0.5 * proj_parm.m;
                    proj_parm.hn = 0.5 * proj_parm.n;
                    par.es = 0.;
                }
            }

        }} // namespace detail::oea
    #endif // doxygen

    /*!
        \brief Oblated Equal Area projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Miscellaneous
         - Spheroid
        \par Projection parameters
         - n (real)
         - m (real)
         - theta: Theta (degrees)
        \par Example
        \image html ex_oea.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct oea_spheroid : public detail::oea::base_oea_spheroid<Geographic, Cartesian, Parameters>
    {
        inline oea_spheroid(const Parameters& par) : detail::oea::base_oea_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::oea::setup_oea(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class oea_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<oea_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void oea_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("oea", new oea_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_OEA_HPP

