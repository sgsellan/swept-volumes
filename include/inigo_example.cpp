#include <igl/writeDMAT.h>
#include <Eigen/Core>


double box(const Eigen::RowVector3d & P, const Eigen::RowVector3d & B){
    Eigen::RowVector3d Q = P.cwiseAbs() - B;
    return Q.cwiseMax(0.0).norm() + std::min(std::max(Q(0),std::max(Q(1),Q(2))),0.0);
}
double sphere(const Eigen::RowVector3d & P, const double s){
    return P.norm() - s;
}


double inigo_example(const Eigen::RowVector3d & P, const double time){
    

    
    Eigen::RowVector3d B;
    B << 1.0, 1.0, 0.5;
    //return std::min(sphere(P,1.0),box(P,B));
    return sphere(P,1.0);
    
    
    
    
    
    
    
    
//    // http://iquilezles.org/www/articles/smin/smin.htm
//    double smin( double a, double b, double k )
//    {
//        double h = max(k-abs(a-b),0.0);
//        return min(a, b) - h*h*0.25/k;
//    }
//
//    // http://iquilezles.org/www/articles/smin/smin.htm
//    vec2 smin( vec2 a, vec2 b, double k )
//    {
//        double h = clamp( 0.5+0.5*(b.x-a.x)/k, 0.0, 1.0 );
//        return mix( b, a, h ) - k*h*(1.0-h);
//    }
//
//    // http://iquilezles.org/www/articles/smin/smin.htm
//    double smax( double a, double b, double k )
//    {
//        double h = max(k-abs(a-b),0.0);
//        return max(a, b) + h*h*0.25/k;
//    }
//
//    // http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
//    double sdSphere( vec3 p, double s )
//    {
//        return length(p)-s;
//    }
//
//    // http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
//    double sdEllipsoid( in vec3 p, in vec3 r ) // approximated
//    {
//        double k0 = length(p/r);
//        double k1 = length(p/(r*r));
//        return k0*(k0-1.0)/k1;
//    }
//
//    vec2 sdStick(vec3 p, vec3 a, vec3 b, double r1, double r2) // approximated
//    {
//        vec3 pa = p-a, ba = b-a;
//        double h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
//        return vec2( length( pa - ba*h ) - mix(r1,r2,h*h*(3.0-2.0*h)), h );
//    }
//
//    // http://iquilezles.org/www/articles/smin/smin.htm
//    vec4 opU( vec4 d1, vec4 d2 )
//    {
//        return (d1.x<d2.x) ? d1 : d2;
//    }
//
//    double href;
//    double hsha;
//
//    vec4 map( in vec3 pos, double atime )
//    {
//        hsha = 1.0;
//
//        double t1 = fract(atime);
//        double t4 = abs(fract(atime*0.5)-0.5)/0.5;
//
//        double p = 4.0*t1*(1.0-t1);
//        double pp = 4.0*(1.0-2.0*t1); // derivative of p
//
//        vec3 cen = vec3( 0.5*(-1.0 + 2.0*t4),
//                         pow(p,2.0-p) + 0.1,
//                         floor(atime) + pow(t1,0.7) -1.0 );
//
//        // body
//        vec2 uu = normalize(vec2( 1.0, -pp ));
//        vec2 vv = vec2(-uu.y, uu.x);
//
//        double sy = 0.5 + 0.5*p;
//        double compress = 1.0-smoothstep(0.0,0.4,p);
//        sy = sy*(1.0-compress) + compress;
//        double sz = 1.0/sy;
//
//        vec3 q = pos - cen;
//        double rot = -0.25*(-1.0 + 2.0*t4);
//        double rc = cos(rot);
//        double rs = sin(rot);
//        q.xy = mat2x2(rc,rs,-rs,rc)*q.xy;
//        vec3 r = q;
//        href = q.y;
//        q.yz = vec2( dot(uu,q.yz), dot(vv,q.yz) );
//
//        vec4 res = vec4( sdEllipsoid( q, vec3(0.25, 0.25*sy, 0.25*sz) ), 2.0, 0.0, 1.0 );
//
//        if( res.x-1.0 < pos.y ) // bounding volume
//        {
//        double t2 = fract(atime+0.8);
//        double p2 = 0.5-0.5*cos(6.2831*t2);
//        r.z += 0.05-0.2*p2;
//        r.y += 0.2*sy-0.2;
//        vec3 sq = vec3( abs(r.x), r.yz );
//
//        // head
//        vec3 h = r;
//        double hr = sin(0.791*atime);
//        hr = 0.7*sign(hr)*smoothstep(0.5,0.7,abs(hr));
//        h.xz = mat2x2(cos(hr),sin(hr),-sin(hr),cos(hr))*h.xz;
//        vec3 hq = vec3( abs(h.x), h.yz );
//           double d  = sdEllipsoid( h-vec3(0.0,0.20,0.02), vec3(0.08,0.2,0.15) );
//        double d2 = sdEllipsoid( h-vec3(0.0,0.21,-0.1), vec3(0.20,0.2,0.20) );
//        d = smin( d, d2, 0.1 );
//        res.x = smin( res.x, d, 0.1 );
//
//        // belly wrinkles
//        {
//        double yy = r.y-0.02-2.5*r.x*r.x;
//        res.x += 0.001*sin(yy*120.0)*(1.0-smoothstep(0.0,0.1,abs(yy)));
//        }
//
//        // arms
//        {
//        vec2 arms = sdStick( sq, vec3(0.18-0.06*hr*sign(r.x),0.2,-0.05), vec3(0.3+0.1*p2,-0.2+0.3*p2,-0.15), 0.03, 0.06 );
//        res.xz = smin( res.xz, arms, 0.01+0.04*(1.0-arms.y)*(1.0-arms.y)*(1.0-arms.y) );
//        }
//
//        // ears
//        {
//        double t3 = fract(atime+0.9);
//        double p3 = 4.0*t3*(1.0-t3);
//        vec2 ear = sdStick( hq, vec3(0.15,0.32,-0.05), vec3(0.2+0.05*p3,0.2+0.2*p3,-0.07), 0.01, 0.04 );
//        res.xz = smin( res.xz, ear, 0.01 );
//        }
//
//        // mouth
//        {
//           d = sdEllipsoid( h-vec3(0.0,0.15+4.0*hq.x*hq.x,0.15), vec3(0.1,0.04,0.2) );
//        res.w = 0.3+0.7*clamp( d*150.0,0.0,1.0);
//        res.x = smax( res.x, -d, 0.03 );
//        }
//
//        // legs
//        {
//        double t6 = cos(6.2831*(atime*0.5+0.25));
//        double ccc = cos(1.57*t6*sign(r.x));
//        double sss = sin(1.57*t6*sign(r.x));
//        vec3 base = vec3(0.12,-0.07,-0.1); base.y -= 0.1/sy;
//        vec2 legs = sdStick( sq, base, base + vec3(0.2,-ccc,sss)*0.2, 0.04, 0.07 );
//        res.xz = smin( res.xz, legs, 0.07 );
//        }
//
//        // eye
//        {
//        double blink = pow(0.5+0.5*sin(2.1*iTime),20.0);
//        double eyeball = sdSphere(hq-vec3(0.08,0.27,0.06),0.065+0.02*blink);
//        res.x = smin( res.x, eyeball, 0.03 );
//
//        vec3 cq = hq-vec3(0.1,0.34,0.08);
//        cq.xy = mat2x2(0.8,0.6,-0.6,0.8)*cq.xy;
//        d = sdEllipsoid( cq, vec3(0.06,0.03,0.03) );
//        res.x = smin( res.x, d, 0.03 );
//
//        double eo = 1.0-0.5*smoothstep(0.01,0.04,length((hq.xy-vec2(0.095,0.285))*vec2(1.0,1.1)));
//        res = opU( res, vec4(sdSphere(hq-vec3(0.08,0.28,0.08),0.060),3.0,0.0,eo));
//        res = opU( res, vec4(sdSphere(hq-vec3(0.075,0.28,0.102),0.0395),4.0,0.0,1.0));
//        }
//        }
//
//        // ground
//        double fh = -0.1 - 0.05*(sin(pos.x*2.0)+sin(pos.z*2.0));
//        double t5f = fract(atime+0.05);
//        double t5i = floor(atime+0.05);
//        double bt4 = abs(fract(t5i*0.5)-0.5)/0.5;
//        vec2  bcen = vec2( 0.5*(-1.0+2.0*bt4),t5i+pow(t5f,0.7)-1.0 );
//
//        double k = length(pos.xz-bcen);
//        double tt = t5f*15.0-6.2831 - k*3.0;
//        fh -= 0.1*exp(-k*k)*sin(tt)*exp(-max(tt,0.0)/2.0)*smoothstep(0.0,0.01,t5f);
//        double d = pos.y - fh;
//
//        // bubbles
//        {
//        vec3 vp = vec3( mod(abs(pos.x),3.0)-1.5,pos.y,mod(pos.z+1.5,3.0)-1.5);
//        vec2 id = vec2( floor(pos.x/3.0), floor((pos.z+1.5)/3.0) );
//        double fid = id.x*11.1 + id.y*31.7;
//        double fy = fract(fid*1.312+atime*0.1);
//        double y = -1.0+4.0*fy;
//        vec3  rad = vec3(0.7,1.0+0.5*sin(fid),0.7);
//        rad -= 0.1*(sin(pos.x*3.0)+sin(pos.y*4.0)+sin(pos.z*5.0));
//        double siz = 4.0*fy*(1.0-fy);
//        double d2 = sdEllipsoid( vp-vec3(0.5,y,0.0), siz*rad );
//
//        d2 -= 0.03*smoothstep(-1.0,1.0,sin(18.0*pos.x)+sin(18.0*pos.y)+sin(18.0*pos.z));
//        d2 *= 0.6;
//        d2 = min(d2,2.0);
//        d = smin( d, d2, 0.32 );
//        if( d<res.x ) { res = vec4(d,1.0,0.0,1.0); hsha=sqrt(siz); }
//        }
//
//        // candy
//        {
//        double fs = 5.0;
//        vec3 qos = fs*vec3(pos.x, pos.y-fh, pos.z );
//        vec2 id = vec2( floor(qos.x+0.5), floor(qos.z+0.5) );
//        vec3 vp = vec3( fract(qos.x+0.5)-0.5,qos.y,fract(qos.z+0.5)-0.5);
//        vp.xz += 0.1*cos( id.x*130.143 + id.y*120.372 + vec2(0.0,2.0) );
//        double den = sin(id.x*0.1+sin(id.y*0.091))+sin(id.y*0.1);
//        double fid = id.x*0.143 + id.y*0.372;
//        double ra = smoothstep(0.0,0.1,den*0.1+fract(fid)-0.95);
//        d = sdSphere( vp, 0.35*ra )/fs;
//        if( d<res.x ) res = vec4(d,5.0,qos.y,1.0);
//        }
//
//        return res.x;
//    }
}
