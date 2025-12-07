#pragma once
#include <iostream>
#include <string>
#include "gameTypes.hpp"

ENGINE_API void writeInt(std::ostream &out, int v);
ENGINE_API void writeBool(std::ostream &out, bool v);
ENGINE_API void writeFloat(std::ostream &out, float v);
ENGINE_API void writeString(std::ostream &out, const std::string &v);

ENGINE_API void readInt(std::istream &in, int &v);
ENGINE_API void readBool(std::istream &in, bool &v);
ENGINE_API void readFloat(std::istream &in, float &v);
ENGINE_API void readString(std::istream &in, std::string &v);

ENGINE_API void writeVariable(std::ostream &out, const Variable &v);
ENGINE_API void readVariable(std::istream &in, Variable &v);

ENGINE_API void writeCondition(std::ostream &out, const Condition &c);
ENGINE_API void readCondition(std::istream &in, Condition &c);

ENGINE_API void writeAction(std::ostream &out, const Action &a);
ENGINE_API void readAction(std::istream &in, Action &a);

ENGINE_API void writeChoice(std::ostream &out, const Choice &c);
ENGINE_API void readChoice(std::istream &in, Choice &c);

ENGINE_API void writePrompt(std::ostream &out, const Prompt &p);
ENGINE_API void readPrompt(std::istream &in, Prompt &p);

ENGINE_API void writeScene(std::ostream &out, const Scene &s);
ENGINE_API void readScene(std::istream &in, Scene &s);

ENGINE_API void writeGame(std::ostream &out, const Game &g);
ENGINE_API void readGame(std::istream &in, Game &g);