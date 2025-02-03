class Comm {
public:
    Comm() {
        // Constructor code here
    }

    ~Comm() {
        // Destructor code here
    }

    void init() {
        // Initialization code here
        Serial.println("Initialized communications");
    }

    void end() {
        // Deinitialization code here
        Serial.println("Deinitialized communications");
    }
};