#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int W=600;
const int H=480;
int speed = 4;
bool field[W][H]={0};

struct player
{ int x,y,dir;
  Color color;
  player(Color c)
  {
    x=rand() % W;
    y=rand() % H;
    color=c;
    dir=rand() % 4;
  }
  void tick()
  {
    if (dir==0) y+=1;
    if (dir==1) x-=1;
    if (dir==2) x+=1;
    if (dir==3) y-=1;

    if (x>=W) x=0;  if (x<0) x=W-1;
    if (y>=H) y=0;  if (y<0) y=H-1;
  }

  Vector3f getColor()
  {return Vector3f(color.r,color.g,color.b);}
};

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(W, H), "The Tron Game!");
    window.setFramerateLimit(60);

    Texture texture;
    texture.loadFromFile("background.jpg");
    Sprite sBackground(texture);

    player p1(Color::Red), p2(Color::Green); 

    Sprite sprite;
    RenderTexture t;
    t.create(W, H);
    t.setSmooth(true);
    sprite.setTexture(t.getTexture());
    t.clear();  t.draw(sBackground);
    
    Font font;font.loadFromFile(Sansation_Bold.ttf);
    Text text("You Win!",font,35);
    text.setPosition(W/2-80,20);
    
    Shader* shader = new Shader;
    shader->loadfromfile("shader.frag",Shader::Fragment);
    shader->setParameter("frag_ScreenResolution", Vector2f(W,H));
    shader->setParameter("frag_LightAttenuation", 100);
    RenderStates states; states.shader = shader;
      
    bool Game=1;

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) if (p1.dir!=2) p1.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) if (p1.dir!=1)  p1.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) if (p1.dir!=0) p1.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) if (p1.dir!=3) p1.dir=0;

        if (Keyboard::isKeyPressed(Keyboard::A)) if (p2.dir!=2) p2.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::D)) if (p2.dir!=1)  p2.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::W)) if (p2.dir!=0) p2.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::S)) if (p2.dir!=3) p2.dir=0;

        if (!Game)    continue;

        for(int i=0;i<speed;i++)
        {
            p1.tick(); p2.tick();
            if (field[p1.x][p1.y]==1) Game=0; 
            if (field[p2.x][p2.y]==1) Game=0;
            field[p1.x][p1.y]=1; 
            field[p2.x][p2.y]=1;
    
            t.display();  
          shader->setParameter("frag_LightOrigin", Vector2f(p1.x,p1.y));
          shader->setParameter("frag_LightColor", p1.getColor());
          t.draw(sprite. states);
          shader->setParameter("frag_LightOrigin", Vector2f(p2.x,p2.y));
          shader->setParameter("frag_LightColor", p2.getColor());
          t.draw(sprite. states);
        }

       ////// draw  ///////
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
