#include "serialization.hpp"

ENGINE_API void writeInt(std::ostream &out, int v)
{
  out.write(reinterpret_cast<const char *>(&v), sizeof(v));
}
ENGINE_API void writeBool(std::ostream &out, bool v)
{
  out.write(reinterpret_cast<const char *>(&v), sizeof(v));
}
ENGINE_API void writeFloat(std::ostream &out, float v)
{
  out.write(reinterpret_cast<const char *>(&v), sizeof(v));
}
ENGINE_API void writeString(std::ostream &out, const std::string &v)
{
  unsigned int size = v.size();
  out.write(reinterpret_cast<const char *>(&size), sizeof(size));
  out.write(v.data(), size);
}

ENGINE_API void readInt(std::istream &in, int &v)
{
  in.read(reinterpret_cast<char *>(&v), sizeof(v));
}
ENGINE_API void readBool(std::istream &in, bool &v)
{
  in.read(reinterpret_cast<char *>(&v), sizeof(v));
}
ENGINE_API void readFloat(std::istream &in, float &v)
{
  in.read(reinterpret_cast<char *>(&v), sizeof(v));
}
ENGINE_API void readString(std::istream &in, std::string &v)
{
  unsigned int size;
  in.read(reinterpret_cast<char *>(&size), sizeof(size));
  v.resize(size);
  in.read(&v[0], size);
}

ENGINE_API void writeVariable(std::ostream &out, const Variable &v)
{
  int type = v.type;
  writeInt(out, type);

  switch (v.type)
  {
  case BOOL:
  {
    bool b = std::get<bool>(v.value);
    writeBool(out, b);
    break;
  }
  case INT:
  {
    int i = std::get<int>(v.value);
    writeInt(out, i);
    break;
  }
  case FLOAT:
  {
    float f = std::get<float>(v.value);
    writeFloat(out, f);
    break;
  }
  case STRING:
  {
    const std::string &s = std::get<std::string>(v.value);
    writeString(out, s);
    break;
  }
  }
}

ENGINE_API void readVariable(std::istream &in, Variable &v)
{
  int type;
  readInt(in, type);
  v.type = static_cast<VariableType>(type);

  switch (v.type)
  {
  case BOOL:
  {
    bool b;
    readBool(in, b);
    v.value = b;
    break;
  }
  case INT:
  {
    int i;
    readInt(in, i);
    v.value = i;
    break;
  }
  case FLOAT:
  {
    float f;
    readFloat(in, f);
    v.value = f;
    break;
  }
  case STRING:
  {
    std::string s;
    readString(in, s);
    v.value = s;
    break;
  }
  }
}

ENGINE_API void writeCondition(std::ostream &out, const Condition &c)
{
  writeBool(out, c.global);
  writeString(out, c.target);
  int check = c.check;
  writeInt(out, check);
  writeVariable(out, c.value);
}

ENGINE_API void readCondition(std::istream &in, Condition &c)
{
  readBool(in, c.global);
  readString(in, c.target);
  int check;
  readInt(in, check);
  c.check = static_cast<ConditionCheck>(check);
  readVariable(in, c.value);
}

ENGINE_API void writeAction(std::ostream &out, const Action &a)
{
  writeBool(out, a.hasCondition);
  if (a.hasCondition)
    writeCondition(out, a.condition);

  writeBool(out, a.global);
  writeString(out, a.target);
  writeVariable(out, a.value);

  writeString(out, a.sceneChange);

  bool hasInline = (a.inlinePrompt != nullptr);
  writeBool(out, hasInline);
  if (hasInline)
    writePrompt(out, *a.inlinePrompt);
}

ENGINE_API void readAction(std::istream &in, Action &a)
{
  readBool(in, a.hasCondition);
  if (a.hasCondition)
    readCondition(in, a.condition);

  readBool(in, a.global);
  readString(in, a.target);
  readVariable(in, a.value);

  readString(in, a.sceneChange);

  bool hasInline;
  readBool(in, hasInline);
  if (hasInline)
  {
    a.inlinePrompt = std::make_shared<Prompt>();
    readPrompt(in, *a.inlinePrompt);
  }
  else
  {
    a.inlinePrompt.reset();
  }
}

ENGINE_API void writeChoice(std::ostream &out, const Choice &c)
{
  writeString(out, c.text);

  unsigned int count = c.actions.size();
  writeInt(out, count);
  for (unsigned int i = 0; i < count; i++)
    writeAction(out, c.actions[i]);
}

ENGINE_API void readChoice(std::istream &in, Choice &c)
{
  readString(in, c.text);

  int count;
  readInt(in, count);
  c.actions.resize(count);
  for (int i = 0; i < count; i++)
    readAction(in, c.actions[i]);
}

ENGINE_API void writePrompt(std::ostream &out, const Prompt &p)
{
  writeString(out, p.promptText);

  unsigned int count = p.choices.size();
  writeInt(out, count);
  for (unsigned int i = 0; i < count; i++)
    writeChoice(out, p.choices[i]);
}

ENGINE_API void readPrompt(std::istream &in, Prompt &p)
{
  readString(in, p.promptText);

  int count;
  readInt(in, count);
  p.choices.resize(count);
  for (int i = 0; i < count; i++)
    readChoice(in, p.choices[i]);
}

ENGINE_API void writeScene(std::ostream &out, const Scene &s)
{
  writeString(out, s.name);

  unsigned int count = s.locals.size();
  writeInt(out, count);

  for (auto &kv : s.locals)
  {
    writeString(out, kv.first);
    writeVariable(out, kv.second);
  }

  writePrompt(out, s.prompt);
}

ENGINE_API void readScene(std::istream &in, Scene &s)
{
  readString(in, s.name);

  int count;
  readInt(in, count);
  s.locals.clear();
  for (int i = 0; i < count; i++)
  {
    std::string key;
    Variable val;
    readString(in, key);
    readVariable(in, val);
    s.locals[key] = val;
  }

  readPrompt(in, s.prompt);
}

ENGINE_API void writeGame(std::ostream &out, const Game &g)
{
  writeString(out, g.currentScene);

  unsigned int sceneCount = g.scenes.size();
  writeInt(out, sceneCount);
  for (auto &kv : g.scenes)
  {
    writeString(out, kv.first);
    writeScene(out, kv.second);
  }

  unsigned int glCount = g.globals.size();
  writeInt(out, glCount);
  for (auto &kv : g.globals)
  {
    writeString(out, kv.first);
    writeVariable(out, kv.second);
  }
}

ENGINE_API void readGame(std::istream &in, Game &g)
{
  readString(in, g.currentScene);

  int sceneCount;
  readInt(in, sceneCount);
  g.scenes.clear();
  for (int i = 0; i < sceneCount; i++)
  {
    std::string key;
    Scene s;
    readString(in, key);
    readScene(in, s);
    g.scenes[key] = s;
  }

  int glCount;
  readInt(in, glCount);
  g.globals.clear();
  for (int i = 0; i < glCount; i++)
  {
    std::string key;
    Variable v;
    readString(in, key);
    readVariable(in, v);
    g.globals[key] = v;
  }
}