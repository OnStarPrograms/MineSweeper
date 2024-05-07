#include "Toolbox.h"
#include "Tile.h"
#include <random>
#include <fstream>

class GameState
{
    public:
        enum PlayStatus{ WIN, LOSS, PLAYING};
        GameState(sf::Vector2i _dimensions = sf::Vector2i(25, 16), int _numberOfMines = 50)
        {
            toolbox.flags = 0;
            MineNum = _numberOfMines;
            //Creates a map of Tile* to the dimensions defined
          
            tiles = new Tile**[_dimensions.y+2];
            for(int i = 0; i < _dimensions.y+2; ++i) {
                tiles[i] = new Tile*[_dimensions.x+2];
            }

            //Creates a map of Bool to the dimensions defined
            BombChecker = new bool*[_dimensions.y+2];
            for(int i = 0; i < _dimensions.y+2; ++i) {
                BombChecker[i] = new bool[_dimensions.x+2];
            }

            width = _dimensions.x;
            height = _dimensions.y;

            toolbox.width = width;
            toolbox.height = height;

            //Initializes bool and Tiles*
            for (int i = 0; i < height+2; i++)
            {
                for (int j = 0; j < width+2; j++)
                {
                    tiles[i][j] = new Tile(sf::Vector2f(j-1, i-1));
                    BombChecker[i][j] = false;
                }
            }

            std::random_device rd; // obtain a random number from hardware
            std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> iaba(1, 25); // define the range
            std::uniform_int_distribution<> jaba(1, 16); // define the range

            int MineCount = MineNum;
            while (MineCount >= 0)
            {
                int ib = iaba(gen);
                int jb = jaba(gen);
                if (BombChecker[jb][ib] == false)
                {
                    BombChecker[jb][ib] = true;
                    MineCount--;
                }
            }

            for (int i = 1; i < height+1; i++)
            {
                for (int j = 1; j < width+1; j++)
                {
                    std::array<Tile*, 8> neighborTiles;
                    neighborTiles[0] = tiles[i-1][j+1];
                    neighborTiles[1] = tiles[i][j+1];
                    neighborTiles[2] = tiles[i+1][j+1];
                    neighborTiles[3] = tiles[i-1][j-1];
                    neighborTiles[4] = tiles[i][j-1];
                    neighborTiles[5] = tiles[i+1][j-1];
                    neighborTiles[6] = tiles[i-1][j];
                    neighborTiles[7] = tiles[i+1][j];
                    tiles[i][j]->setNeighbors(neighborTiles);
                }
            }

            //Sets the Bomb map into the toolbox
            toolbox.BombMap = BombChecker;
        };
        GameState(const char* filepath, sf::Vector2i _dimensions = sf::Vector2i(25, 16))
        {
            toolbox.flags = 0;
            tiles = new Tile**[_dimensions.y+2];
            for(int i = 0; i < _dimensions.y+2; ++i) {
                tiles[i] = new Tile*[_dimensions.x+2];
            }

            //Creates a map of Bool to the dimensions defined
            BombChecker = new bool*[_dimensions.y+2];
            for(int i = 0; i < _dimensions.y+2; ++i) {
                BombChecker[i] = new bool[_dimensions.x+2];
            }

            width = _dimensions.x;
            height = _dimensions.y;

            toolbox.width = width;
            toolbox.height = height;

            for (int i = 0; i < height+2; i++)
            {
                for (int j = 0; j < width+2; j++)
                {
                    tiles[i][j] = new Tile(sf::Vector2f(j-1, i-1));
                    BombChecker[i][j] = false;
                }
            }

            MineNum = 0;
            std::ifstream myfile;
            myfile.open(filepath);
            std::string line;
            if (myfile.is_open())
            {
                int i = 1;
                while ( std::getline(myfile,line) )
                {
                    if (i < 17)
                    {
                        for (int j = 0; j < 25; j++)
                        {
                            if (line[j] == '0')
                            BombChecker[i][j+1] = 0;
                            else
                            {
                                BombChecker[i][j+1] = 1;
                                MineNum++;
                            }
                        }
                        i++;
                    }
                }
                myfile.close();
            }
            for (int i = 1; i < height+1; i++)
            {
                for (int j = 1; j < width+1; j++)
                {
                    std::array<Tile*, 8> neighborTiles;
                    neighborTiles[0] = tiles[i-1][j+1];
                    neighborTiles[1] = tiles[i][j+1];
                    neighborTiles[2] = tiles[i+1][j+1];
                    neighborTiles[3] = tiles[i-1][j-1];
                    neighborTiles[4] = tiles[i][j-1];
                    neighborTiles[5] = tiles[i+1][j-1];
                    neighborTiles[6] = tiles[i-1][j];
                    neighborTiles[7] = tiles[i+1][j];
                    tiles[i][j]->setNeighbors(neighborTiles);
                }
            }

            toolbox.BombMap = BombChecker;
            
        };
        ~GameState()
        {
            for (int i = 0; i < height; i++)
            delete[] BombChecker[i];
            delete[] BombChecker;
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < height; i++)
                {
                    delete tiles[i][j];
                }
                delete[] tiles[j];
            }
            delete[] tiles;
        };

        int getFlagCount()
        {
            return toolbox.flags;
        };
        int getMineCount()
        {
            return MineNum;
        };
        Tile* getTile(int x, int y)
        {
            if (tiles[x][y]->getState() == Tile::EXPLODED)
            {
                Status = LOSS;
            }
            return tiles[x][y];
        };
        PlayStatus getPlayStatus()
        {
            return Status;
        };
        void setPlayStatus(PlayStatus _status)
        {
            Status = _status;
        };
    private:
        /* data */
        Toolbox &toolbox = Toolbox::getInstance();
        PlayStatus Status = PLAYING;
        Tile*** tiles;
        bool** BombChecker;
        int width;
        int height;
        int flagCount = 0;
        int MineNum;
};
