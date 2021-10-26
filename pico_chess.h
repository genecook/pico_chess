namespace PicoChess {

  class Engine {
  public:
    Engine() {};
    ~Engine() {};
    void Load(std::string &load_file) {};
    void Save(std::string save_file) {};
    void SetDebug(bool debug_state) {};
    void UserMove(std::string &usermove_err_msg,std::string usermove) {};
    void NextMove(std::string &usermove_err_msg) {};
    void ShowBoard() {};
    void NewGame() {};
    void ChangeSides() {};
    void SetColor(std::string color) {};
    
  private:

  };

};
