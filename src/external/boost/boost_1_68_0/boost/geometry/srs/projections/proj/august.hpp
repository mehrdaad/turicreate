// Boost.Geometry - gis-projections (based on PROJ4)

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2017, 2018.
// Modifications copyright (c) 2017-2018, Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 5.0.0

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

#ifndef BOOST_GEOMETRY_PROJECTIONS_AUGUST_HPP
#define BOOST_GEOMETRY_PROJECTIONS_AUGUST_HPP

#include <boost/geometry/srs/projections/impl/base_static.hpp>
#include <boost/geometry/srs/projections/impl/base_dynamic.hpp>
#include <boost/geometry/srs/projections/impl/projects.hpp>
#include <boost/geometry/srs/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry
{

namespace srs { namespace par4
{
    struct august {};

}} //namespace srs::par4

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace august
    {

            //static const double M = 1.333333333333333;

            // template class, using CRTP to implement forward/inverse
            template <typename T, typename Parameters>
            struct base_august_spheroid
                : public base_t_f<base_august_spheroid<T, Parameters>, T, Parameters>
            {
                inline base_august_spheroid(const Parameters& par)
                    : base_t_f<base_august_spheroid<T, Parameters>, T, Parameters>(*this, par)
                {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(T& lp_lon, T& lp_lat, T& xy_x, T& xy_y) const
                {
                    static const T M = 1.333333333333333333333333333333333333;

                    T t, c1, c, x1, x12, y1, y12;

                    t = tan(.5 * lp_lat);
                    c1 = sqrt(1. - t * t);
                    c = 1. + c1 * cos(lp_lon *= .5);
                    x1 = sin(lp_lon) *  c1 / c;
                    y1 =  t / c;
                    xy_x = M * x1 * (3. + (x12 = x1 * x1) - 3. * (y12 = y1 *  y1));
                    xy_y = M * y1 * (3. + 3. * x12 - y12);
                }

                static inline std::string get_name()
                {
                    return "august_spheroid";
                }

            };

            // August Epicycloidal
            template <typename Parameters>
            inline void setup_august(Parameters& par)
            {
                par.es = 0.;
            }

    }} // namespace detail::august
    #endif // doxygen

    /*!
        \brief August Epicycloidal projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Miscellaneous
         - Spheroid
         - no inverse
        \par Example
        \image html ex_august.gif
    */
    template <typename T, typename Parameters>
    struct august_spheroid : public detail::august::base_august_spheroid<T, Parameters>
    {
        inline august_spheroid(const Parameters& par) : detail::august::base_august_spheroid<T, Parameters>(par)
        {
            detail::august::setup_august(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::par4::august, august_spheroid, august_spheroid)

        // Factory entry(s)
        template <typename T, typename Parameters>
        class august_entry : public detail::factory_entry<T, Parameters>
        {
            public :
                virtual base_v<T, Parameters>* create_new(const Parameters& par) const
                {
                    return new base_v_f<august_spheroid<T, Parameters>, T, Parameters>(par);
                }
        };

        template <typename T, typename Parameters>
        inline void august_init(detail::base_factory<T, Parameters>& factory)
        {
            factory.add_to_factory("august", new august_entry<T, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_AUGUST_HPP
