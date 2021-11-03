#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include <cstdint>

template<typename baseType>
class AVLNode { //Класс узел АВЛ-дерева
private:
    // Тип - указатель на АВЛ узел с базовым типом baseType
    typedef AVLNode<baseType> *nodePtr;

    baseType _data; //Данные, которые хранит узел
    uint8_t _height; //Высота поддерева с корнем в данном узле
public:
    nodePtr leftChild; //Левый потомок
    nodePtr rightChild; //Правый потомок

    // Конструктор узла АВЛ-дерева по-умолчанию
    AVLNode() = default;

    // Создает узел хранящий данные data
    explicit AVLNode(baseType data);


    //Возвращает данные, которые хранит узел
    inline baseType data();

    // Возвращает высоту поддерева с корнем в данном узле
    inline uint8_t height(nodePtr node);

    // Возвращает фактор баланса узла, если он равен 2 или -2 -
    // поддерево с корнем в этом узле разбалансированно
    uint8_t balanceFactor();

    // Восстанавливает корректное значение поля высоты поддеревьев данного узла
    void fixHeight();

    // Выполняет правый поворот вокруг узла localRoot, возвращает указатель на
    // узел, который встает на место узла LocalRoot
    nodePtr rightRotate(nodePtr localRoot);

    // Выполняет левый поворот вокруг узла localRoot, возвращает указатель на
    // узел, который встает на место узла LocalRoot
    nodePtr leftRotate(nodePtr localRoot);

    // Выполняет балансировку поддерева с корнем в данном узле,
    // возвращает указатель на сбаласированный узел
    nodePtr balance();
}; // Конец класса AVLNode

template<typename baseType>
class AVLTree { // Класс АВЛ-дерево
private:
    // Тип - указатель на АВЛ узел с базовым типом baseType
    typedef AVLNode<baseType> *nodePtr;

    nodePtr root; //Корень дерева
public:
    //Конструктор АВЛ-дерева
    AVLTree();

    //Деструктор АВЛ дерева
    ~AVLTree();

    // Вставляет узел с данными data в дерево
    void insert(baseType data);

    // Удаление узла хранящего данные key из дерева
    void remove(baseType data);

    // Возвращает значение ИСТИНА, если узел с ключем key есть в дереве,
    // иначе возвращает ЛОЖЬ
    bool find(baseType data);

    // Возвращает максимальное значение ключа в дереве
    baseType max();

    // Возвращает минимальное значение ключа в дереве
    baseType min();

    // Выполняет симметричный обход дерева,
    // вызывая для каждого узла функцию function
    void inOrder(void (*function)(baseType));
private:
    // Выполняет симметричный обход начиная с localRoot, вызывая для каждого
    // узла функцию function
    void _inOrder(nodePtr localRoot, void (*function)(baseType));

    /* Вставляет узел с данными data в поддерево с корнем localRoot, выполняя
     * при этом балансировку дерева, возвращает указатель на корень
     * дерева со вставленным узлом */
    nodePtr _insert(nodePtr localRoot, baseType data);

    /* Удаляет узел с данными data из поддерева с корнем localRoot, выполняя
     * при этом балансировку дерева, возвращает указатель на корень
     * поддерева без удаленного узла */
    nodePtr _remove(nodePtr localRoot, baseType key);

    /* Удаляет узел с минимальным ключем из поддерева c корнем localRoot,
     * выполняя баласировку дерева, возвращает указатель на правый узел
     * удаленного узла */
    nodePtr removeMin(nodePtr localRoot);

    // Возвращает указатель на узел с минимальным ключем
    // в поддереве с корнем localRoot
    nodePtr _min(nodePtr localRoot);

    // Возвращает указатель на узел с максимальным ключем
    // в поддереве с корнем localRoot
    nodePtr _max(nodePtr localRoot);

    //Освобождает память, которую занимает поддерево с корнем в узле localRoot
    void deleteTree(nodePtr localRoot);
}; // Конец класса AVLTree

#include "AVLTree.tpp"

#endif //AVLTREE_AVLTREE_H
