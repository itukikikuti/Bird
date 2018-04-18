#include <cmath>
#include <string>
#include "XLibrary11.hpp"

using namespace std;
using namespace XLibrary11;

enum Mode
{
    Title, Game
};

int MAIN()
{
    const int blockNumber = 10;
    const float blockInterval = 200.0f;
    const float blockSpace = 200.0f;
    const float gravitySpeed = 0.5f;
    const float jumpSpeed = 10.0f;
    const float speed = 2.0f;

    float blockOffset = 0.0f;
    float position = 0.0f;
    float gravity = 0.0f;
    int score = 0;
    Mode mode = Title;

    App::SetTitle(L"‚Í‚Î‚½‚¯’¹");

    Camera camera;
    camera.color = Float4(0.5f, 0.75f, 1.0f, 1.0f);

    Sprite playerSprite(L"player.png");
    playerSprite.scale = 3.0f;

    Sprite blockSprite(L"block.png");
    blockSprite.scale = 5.0f;

    Text scoreText(L"0", 16.0f);
    scoreText.position.y = 200.0f;
    scoreText.scale = 5.0f;
    scoreText.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

    Text titleText(L"‚Í‚Î‚½‚¯’¹", 32.0f);
    titleText.position.x = 100.0f;
    titleText.scale = 2.0f;
    titleText.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

    Sound flySound(L"fly.wav");
    Sound hitSound(L"hit.wav");
    Sound pointSound(L"point.wav");

    Float2 blockPosition[blockNumber];

    while (App::Refresh())
    {
        camera.Update();

        switch (mode)
        {
        case Title:

            if (App::GetKeyDown(VK_SPACE))
            {
                blockOffset = 0.0f;
                position = 0.0f;
                gravity = 0.0f;
                score = 0;
                mode = Game;
                scoreText.Create(L"0", 16.0f);

                for (int i = 0; i < blockNumber; i++)
                {
                    blockPosition[i] = Float2(blockOffset + blockInterval, rand() / (float)RAND_MAX * blockSpace - blockSpace / 2);
                    blockOffset = blockPosition[i].x;
                }
            }

            playerSprite.position = Float3(-200.0f, 0.0f, 0.0f);
            position = playerSprite.position.x;
            playerSprite.angles.z = 0.0f;
            playerSprite.Draw();

            titleText.Create(L"‚Í‚Î‚½‚¯’¹", 32.0f);
            titleText.Draw();

            scoreText.Create(to_wstring(score), 16.0f);
            scoreText.Draw();

            break;

        case Game:

            for (int i = 0; i < blockNumber; i++)
            {
                if (blockPosition[i].x < -App::GetWindowSize().x)
                {
                    blockPosition[i] = Float2(blockOffset + blockInterval, rand() / (float)RAND_MAX * blockSpace - blockSpace / 2);
                    blockOffset = blockPosition[i].x;
                }

                float playerSize = playerSprite.GetSize().x * playerSprite.scale.x * 0.8f;
                float blockWidth = (blockSprite.GetSize().x * blockSprite.scale.x + playerSize) / 2.0f;
                float blockHeight = (blockSpace - playerSize) / 2.0f;

                if (blockPosition[i].x - blockWidth < playerSprite.position.x &&
                    blockPosition[i].x + blockWidth > playerSprite.position.x)
                {
                    if (blockPosition[i].y - blockHeight > playerSprite.position.y ||
                        blockPosition[i].y + blockHeight < playerSprite.position.y)
                    {
                        mode = Title;
                        hitSound.Play();
                    }
                }

                blockPosition[i].x -= speed;

                blockSprite.position = blockPosition[i];
                blockSprite.position.y += (blockSprite.GetSize().y * blockSprite.scale.y + blockSpace) / 2.0f;
                blockSprite.Draw();

                blockSprite.position = blockPosition[i];
                blockSprite.position.y -= (blockSprite.GetSize().y * blockSprite.scale.y + blockSpace) / 2.0f;
                blockSprite.Draw();
            }

            blockOffset -= speed;

            gravity -= gravitySpeed;

            if (App::GetKeyDown(VK_SPACE))
            {
                gravity = jumpSpeed;
                flySound.Play();
            }

            playerSprite.position.y += gravity;
            playerSprite.angles.z = gravity * 5.0f;
            playerSprite.Draw();

            position += speed;

            if (position > blockInterval)
            {
                position -= blockInterval;
                score++;
                scoreText.Create(to_wstring(score), 16.0f);
                pointSound.Play();
            }

            scoreText.Draw();

            break;
        }
    }

    return 0;
}
