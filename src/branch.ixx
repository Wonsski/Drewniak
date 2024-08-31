export module branch;

import game_object;

export class Branch : public GameObject {

public:
    Branch(float x, float y, const std::string& spritePath, int branchType);
    void chop(int height);
    int type;
};

Branch::Branch(float x, float y, const std::string& spritePath, int branchType)
    : GameObject(x, y, spritePath) {
    type = branchType;
}

void Branch::chop(int height) {
    this->positionY += height;
}