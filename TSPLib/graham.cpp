//
// This program uses the Graham scan algorithm to calculate the convex
// hull of a batch of points
//

#include "tsplib_global.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <string>

class GrahamScan
{
public:
    void debug()
    {
        size_t i ;
        qDebug("%s", "lower");
        for(i=0; i<lower_hull.size(); i++)
        {
            qDebug("%f, %f", lower_hull[i].first, lower_hull[i].second);
        }
        qDebug("%s", "upper");
        for(i=0; i<upper_hull.size(); i++)
        {
            qDebug("%f, %f", upper_hull[i].first, upper_hull[i].second);
        }
    }

    size_t*
    getIndices()
    {

    }

    GrahamScan( size_t n, Point * points )
        : N( n )
    {
        for ( size_t i = 0 ; i < N ; i++ ) {
            raw_points.push_back( std::make_pair( points[i].x, points[i].y ) );
        }
    }
    //
    // The initial array of points is stored in vectgor raw_points. I first
    // sort it, which gives me the far left and far right points of the hull.
    // These are special values, and they are stored off separately in the left
    // and right members.
    //
    // I then go through the list of raw_points, and one by one determine whether
    // each point is above or below the line formed by the right and left points.
    // If it is above, the point is moved into the upper_partition_points sequence. If it
    // is below, the point is moved into the lower_partition_points sequence. So the output
    // of this routine is the left and right points, and the sorted points that are in the
    // upper and lower partitions.
    //
    void partition_points()
    {
        //
        // Step one in partitioning the points is to sort the raw data
        //
        std::sort( raw_points.begin(), raw_points.end() );
        //
        // The the far left and far right points, remove them from the
        // sorted sequence and store them in special members
        //
        left = raw_points.front();
        raw_points.erase( raw_points.begin() );
        right = raw_points.back();
        raw_points.pop_back();
        //
        // Now put the remaining points in one of the two output sequences
        //

        size_t i ;
        qDebug("%s", "raw_points");
        for(i=0; i<raw_points.size(); i++)
        {
            qDebug("%f, %f", raw_points[i].first, raw_points[i].second);
        }
        for ( size_t i = 0 ; i < raw_points.size() ; i++ )
        {
            double dir = direction( left, right, raw_points[ i ] );
            if ( dir < 0 )
                upper_partition_points.push_back( raw_points[ i ] );
            else
                lower_partition_points.push_back( raw_points[ i ] );
        }
    }
    //
    // Building the hull consists of two procedures: building the lower and
    // then the upper hull. The two procedures are nearly identical - the main
    // difference between the two is the test for convexity. When building the upper
    // hull, our rull is that the middle point must always be *above* the line formed
    // by its two closest neighbors. When building the lower hull, the rule is that point
    // must be *below* its two closest neighbors. We pass this information to the
    // building routine as the last parameter, which is either -1 or 1.
    //
    void build_hull( )
    {
        build_half_hull( lower_partition_points, lower_hull, 1 );
        build_half_hull( upper_partition_points, upper_hull, -1 );
    }
    //
    // This is the method that builds either the upper or the lower half convex
    // hull. It takes as its input a copy of the input array, which will be the
    // sorted list of points in one of the two halfs. It produces as output a list
    // of the points in the corresponding convex hull.
    //
    // The factor should be 1 for the lower hull, and -1 for the upper hull.
    //
    void build_half_hull( std::vector< std::pair<double,double> > input,
                          std::vector< std::pair<double,double> > &output,
                          int factor )
    {
        //
        // The hull will always start with the left point, and end with the right
        // point. According, we start by adding the left point as the first point
        // in the output sequence, and make sure the right point is the last point
        // in the input sequence.
        //
        input.push_back( right );
        output.push_back( left );
        //
        // The construction loop runs until the input is exhausted
        //
        while ( input.size() != 0 ) {
            //
            // Repeatedly add the leftmost point to the null, then test to see
            // if a convexity violation has occured. If it has, fix things up
            // by removing the next-to-last point in the output suqeence until
            // convexity is restored.
            //
            output.push_back( input.front() );
            input.erase( input.begin() );
            while ( output.size() >= 3 ) {
                size_t end = output.size() - 1;
                if ( factor * direction( output[ end - 2 ],
                                         output[ end ],
                                         output[ end - 1 ] ) <= 0 ) {
                    output.erase( output.begin() + end - 1 );
                }
                else
                    break;
            }
        }
    }
    //
    // In this program we frequently want to look at three consecutive
    // points, p0, p1, and p2, and determine whether p2 has taken a turn
    // to the left or a turn to the right.
    //
    // We can do this by by translating the points so that p1 is at the origin,
    // then taking the cross product of p0 and p2. The result will be positive,
    // negative, or 0, meaning respectively that p2 has turned right, left, or
    // is on a straight line.
    //
    static double direction( std::pair<double,double> p0,
                          std::pair<double,double> p1,
                          std::pair<double,double> p2 )
    {
        return ( (p0.first - p1.first ) * (p2.second - p1.second ) )
               - ( (p2.first - p1.first ) * (p0.second - p1.second ) );
    }
private :
    const size_t N;
    // The raw data points generated by the constructor
    std::vector< std::pair<double,double> > raw_points;
    //
    // These values are used to represent the partitioned set. A special
    // leftmost and rightmost value, and the sorted set of upper and lower
    // partitioned points that lie inside those two points.
    //
    std::pair<double,double> left;
    std::pair<double,double> right;
    std::vector< std::pair<double,double> > upper_partition_points;
    std::vector< std::pair<double,double> > lower_partition_points;
    //
    // After the convex hull is created, the lower hull and upper hull
    // are stored in these sorted sequences. There is a bit of duplication
    // between the two, because both sets include the leftmost and rightmost point.
    //
    std::vector< std::pair<double,double> > lower_hull;
    std::vector< std::pair<double,double> > upper_hull;
};

/*
int main(int argc, char* argv[])
{
    std::ofstream gnuplot_file( "gnuplot.cmd" );
    const int N = 20;
    GrahamScan g( N, 0, 100, 0, 100 );
    g.log_raw_points( std::cout );
    g.plot_raw_points( gnuplot_file );
    g.partition_points();
    g.log_partitioned_points( std::cout );
    g.plot_partitioned_points( gnuplot_file );
    //
    // Okay, time to start building the hull
    //
    g.build_hull( gnuplot_file );
    g.log_hull( std::cout );
    g.plot_hull( gnuplot_file, "complete" );
    return 0;
}
*/
