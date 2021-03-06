/**
* @file Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include <GL/glew.h>
#include "Mesh.h"
#include "Collision.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <functional>

class ActorList;
class Actor;
using ActorPtr = std::shared_ptr<Actor>;

/**
* シーンに配置するオブジェクト.
*/
class Actor
{
public:
  Actor(const std::string& name, int hp, const glm::vec3& pos,
    const glm::vec3& rot = glm::vec3(0), const glm::vec3& scale = glm::vec3(1));
  virtual ~Actor() = default;

  virtual void Update(float);
  virtual void UpdateDrawData(float);
  virtual void Draw();

  virtual void OnHit(const ActorPtr&, const glm::vec3&) {}

public:
  std::string name;
  glm::vec3 position = glm::vec3(0);
  glm::vec3 rotation = glm::vec3(0);
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 velocity = glm::vec3(0);
  glm::vec4 color = glm::vec4(1);
  int health = 0;
  Collision::Shape colLocal;
  Collision::Shape colWorld;
};

/**
* 静的メッシュ用アクター.
*/
class StaticMeshActor : public Actor
{
public:
  StaticMeshActor(const Mesh::MeshPtr& m, const std::string& name, int hp, const glm::vec3& pos,
    const glm::vec3& rot = glm::vec3(0), const glm::vec3& scale = glm::vec3(1));
  virtual ~StaticMeshActor() = default;

  virtual void Draw() override;

  const Mesh::MeshPtr& GetMesh() const { return mesh; }

private:
  Mesh::MeshPtr mesh;
};
using StaticMeshActorPtr = std::shared_ptr<StaticMeshActor>;

/**
* アクターをまとめて操作するクラス.
*/
class ActorList
{
public:
  using iterator = std::vector<ActorPtr>::iterator;
  using const_iterator = std::vector<ActorPtr>::const_iterator;

  ActorList() = default;
  ~ActorList() = default;

  void Reserve(size_t);
  void Add(const ActorPtr&);
  bool Remove(const ActorPtr&);
  void RemoveDead();
  void Clear();
  void Update(float);
  void UpdateDrawData(float);
  void Draw();
  ActorPtr Find(const std::string& name) const;

  iterator begin() { return actors.begin(); }
  iterator end() { return actors.end(); }
  const_iterator begin() const { return actors.begin(); }
  const_iterator end() const { return actors.end(); }

private:
  std::vector<ActorPtr> actors;
};

using CollsionHandlerType = std::function<void(const ActorPtr&, const ActorPtr&, const glm::vec3&)>;
bool DetectCollision(const ActorPtr& a, const ActorPtr& b, CollsionHandlerType handler = nullptr);
void DetectCollision(const ActorPtr& a, const ActorList& b, CollsionHandlerType handler = nullptr);
void DetectCollision(const ActorList& a, const ActorList& b, CollsionHandlerType handler = nullptr);

#endif // ACTOR_H_INCLUDED