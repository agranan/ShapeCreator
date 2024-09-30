#include <iostream>
#include <simplecpp>
using namespace std;

double calcDist(double xx1, double yy1, double xx2, double yy2)
{
    double dist = sqrt(pow(xx1-xx2, 2) + pow(yy1-yy2,2));
    return dist;
}

class MyShape
{
    public:
        virtual void drawShape() = 0;
        virtual bool checkifinBounds(double, double) = 0;
        virtual void moveShape(int, int) = 0;
};


class MyLine: public MyShape
{
    private:
        double mX1, mY1, mX2, mY2;
        Line mLL;

    public:
        MyLine(double x1, double y1, double x2, double y2)
        {
            mX1 = x1;
            mY1 = y1;
            mX2 = x2;
            mY2 = y2;
        }

        virtual void drawShape()
        {
            mLL = Line(mX1, mY1, mX2, mY2);
        }

        virtual bool checkifinBounds(double x, double y)
        {
            double dist = abs(((mY2-mY1)*x +
                            (mX1-mX2)*y +
                             mY1*(mX2-mX1) - 
                             mX1*(mY2-mY1))/
                             sqrt(pow((mX2-mX1),2) +pow(mY2-mY1,2)));
            if(dist <= 5)
                return true;

            return false;
        }

        virtual void moveShape(int lx, int ly)
        {
            double slope = (mY2-mY1)/(mX2-mX1);
            double theta = atan(slope);

            if(theta < 0)
                theta = theta + 3.141;
            
            double len = calcDist(mX1, mY1, mX2, mY2);
            double fx = (mY1-mY2)*((mY2-mY1)*lx +
                         (mX1-mX2)*ly + mY1*(mX2-mX1) - 
                         mX1*(mY2-mY1))/(pow((mX2-mX1),2) + 
                         pow(mY2-mY1,2)) + lx;
            double fy = (mX2-mX1)*((mY2-mY1)*lx + 
                        (mX1-mX2)*ly + mY1*(mX2-mX1) - 
                         mX1*(mY2-mY1))/(pow((mX2-mX1),2) + 
                         pow(mY2-mY1,2)) + ly;

            double d1 = calcDist(fx, fy, mX1, mY1);
            double d2 = calcDist(fx, fy, mX2, mY2);

            mX1 = lx + d1*cos(theta);
            mY1 = ly + d1*sin(theta);
            mX2 = lx + d2*cos(theta);
            mY2 = ly + d2*sin(theta);

            mLL.reset(mX1, mY1, mX2, mY2);
            
            cout << mX1 << " " << mY1 << " " << mX2 << " " << mY2 << endl;
        }

};

class MyCircle: public MyShape
{
    private:
        double mX, mY, mR;
        Circle mCC;

    public:
        MyCircle(double xx, double yy, double rr)
        {
            mX = xx;
            mY = yy;
            mR = rr;
        }

        virtual void drawShape()
        {
            mCC = Circle(mX, mY, mR);
        }

        virtual bool checkifinBounds(double x, double y)
        {
            double dist = sqrt(pow(mX - x, 2) + pow(mY - y, 2));
            if(dist <= mR)
            {
                return true;
            }
            return false;
        }

        virtual void moveShape(int lx, int ly)
        {
            mCC.move(lx, ly);
            mX = mCC.getX();
            mY = mCC.getY();
            cout << "mX" << mX << " " << mY << endl; 
        }

        void getCenter(int &x, int &y)
        {
            x = mX;
            y = mY;
        }
};


int main()
{
    initCanvas("MoveCircle", 800, 800);
    int lastx = 0, lasty = 0;

    MyCircle c1(300, 300, 30), c2(100, 100, 50), c3(400, 400, 30);
    MyLine l1(20, 20, 50, 50), l2(100, 100, 100, 400), l3(300, 300, 45, 78);
    MyCircle arr[3] = {c1, c2, c3};

    XEvent event;

    for(int i = 0; i < 3; i++)
    {
        arr[i].drawShape();
    }

    int clickedIndex = -1;
    while(1)
    {
        nextEvent(event);
        if(mouseButtonPressEvent(event))
        {
            cout << "I got" << endl;
            for(int i = 0; i < 3; i++)
            {
                bool b = arr[i].checkifinBounds(event.xmotion.x, event.xmotion.y);
                cout << b << endl;
                if(b)
                {
                    clickedIndex = i;
                    cout << i << "COMING FROM I" << endl;
                    lastx = event.xmotion.x;
                    lasty = event.xmotion.y;
                    break;
                }
              
            }
        }

        if(mouseDragEvent(event))
        {
            if(clickedIndex > -1)
            {
                arr[clickedIndex].moveShape(event.xmotion.x - lastx, 
                                 event.xmotion.y - lasty);
                lastx = event.xmotion.x;
                lasty = event.xmotion.y;
            }
        }
    }
    
    return 0;
}
