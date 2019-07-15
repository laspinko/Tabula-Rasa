class scene
{
public:
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void handleEvent(SDL_Event) = 0;
};

class game: public scene
{
public:
    int mouseX, mouseY;
    void init();
    void draw();
    void handleEvent(SDL_Event e);
};
