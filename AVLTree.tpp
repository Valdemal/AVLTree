// Реализация узла AВЛ-дерева

template <typename baseType>
AVLNode<baseType>::AVLNode(baseType data) {
    this->_data = data;
    this->leftChild = this->rightChild = nullptr;
    this->_height = 1;
}

template <typename baseType>
inline baseType AVLNode<baseType>::data() {
    return _data;
}

// Возвращает высоту поддерева с корнем в данном узле
template <typename baseType>
inline uint8_t AVLNode<baseType>::height(nodePtr node) {
    return node != nullptr ? node->_height : 0;
}

// Возвращает фактор баланса узла, если он равен 2 или -2 -
// поддерево с корнем в этом узле разбалансированно
template <typename baseType>
uint8_t AVLNode<baseType>::balanceFactor() {
    return height(this->rightChild) - height(this->leftChild);
}

template <typename baseType>
void AVLNode<baseType>::fixHeight() {
    size_t leftHeight = height(this->leftChild);
    size_t rightHeight = height(this->rightChild);
    this->_height = (leftHeight > rightHeight ?
                     leftHeight : rightHeight) + 1;
}

// Выполняет балансировку поддерева с корнем в данном узле,
// возвращает указатель на сбаласированный узел
template <typename baseType>
AVLNode<baseType>* AVLNode<baseType>::balance() {
    this->fixHeight();
    uint8_t bFactor = this->balanceFactor();

    if (bFactor == 2) {
        if (this->rightChild->balanceFactor() < 0)
            this->rightChild = rightRotate(this->rightChild);
        return leftRotate(this);
    }
    if (bFactor == -2) {
        if (this->leftChild->balanceFactor() > 0)
            this->leftChild = leftRotate(this->leftChild);
        return rightRotate(this);
    }
    return this;
}

// Выполняет правый поворот вокруг узла localRoot, возвращает указатель на
// узел, который встает на место узла LocalRoot
template <typename baseType>
AVLNode<baseType>* AVLNode<baseType>::rightRotate(nodePtr localRoot) {
    nodePtr newRoot = localRoot->leftChild;
    localRoot->leftChild = newRoot->rightChild;
    newRoot->rightChild = localRoot;
    localRoot->fixHeight();
    newRoot->fixHeight();
    return newRoot;
}

// Выполняет левый поворот вокруг узла localRoot, возвращает указатель на
// узел, который встает на место узла LocalRoot
template <typename baseType>
AVLNode<baseType>* AVLNode<baseType>::leftRotate(nodePtr localRoot) {
    nodePtr newRoot = localRoot->rightChild;
    localRoot->rightChild = newRoot->leftChild;
    newRoot->leftChild = localRoot;
    localRoot->fixHeight();
    newRoot->fixHeight();
    return newRoot;
}
// Конец реализации узла AВЛ-дерева


// Реализация АВЛ-дерева

template <typename baseType>
AVLTree<baseType>::AVLTree() {
    root = nullptr;
}

// РЕАЛИЗАЦИЯ ПОИСКА

template<typename baseType>
bool AVLTree<baseType>::find(baseType key) {
    nodePtr current = root;
    while (current->data() != key) {
        if (key < current->data())
            current = current->leftChild;
        else
            current = current->rightChild;

        if (current == nullptr)
            return false;
    }
    return true;
}

template<typename baseType>
baseType AVLTree<baseType>::min() {
    return _min(root)->data();
}

template<typename baseType>
baseType AVLTree<baseType>::max() {
    return _max(root)->data();
}

// Возвращает указатель на узел с минимальным ключем
// в поддереве с корнем localRoot
template <typename baseType>
AVLNode<baseType>* AVLTree<baseType>::_min(nodePtr localRoot) {
    return localRoot->leftChild ?
           _min(localRoot->leftChild) : localRoot;
}

template <typename baseType>
AVLNode<baseType>* AVLTree<baseType>::_max(nodePtr localRoot) {
    return localRoot->rightChild ?
           _max(localRoot->rightChild) : localRoot;
}

// РЕАЛИЗАЦИЯ ОБХОДА

template<typename baseType>
void AVLTree<baseType>::inOrder(void (*function)(baseType)) {
    _inOrder(root, function);
}

template<typename baseType>
inline void AVLTree<baseType>::_inOrder(nodePtr localRoot,
                                        void (*function)(baseType)) {
    if (localRoot != nullptr) {
        _inOrder(localRoot->leftChild, function);
        function(localRoot->data());
        _inOrder(localRoot->rightChild, function);
    }
}

// РЕАЛИЗАЦИЯ ВСТАВКИ

// Вставляет узел с данными data в дерево
template <typename baseType>
void AVLTree<baseType>::insert(baseType data) {
    root = _insert(root, data);
}

/* Вставляет узел с данными data в поддерево с корнем localRoot, выполняя
* при этом балансировку дерева, возвращает указатель на корень
* дерева со вставленным узлом */
template <typename baseType>
AVLNode<baseType>* AVLTree<baseType>::_insert(nodePtr localRoot,
                                              baseType data) {
    if (!localRoot)
        // Если текущий узел пустой, значит место для вставки найдено
        // размещаем узел в памяти и возвращаем указатель на него, он
        // вставится при рекурсивном подъеме
        return new AVLNode<baseType>(data);

    if (data < localRoot->data())
        localRoot->leftChild = _insert(localRoot->leftChild, data);
    else
        localRoot->rightChild = _insert(localRoot->rightChild, data);

    // Балансировка узла при выходе из рекурсии
    return localRoot->balance();
}

// РЕАЛИЗАЦИЯ УДАЛЕНИЯ

// Удаление узла хранящего данные key из дерева
template <typename baseType>
void AVLTree<baseType>::remove(baseType data) {
    root = _remove(root, data);
}

/* Удаляет узел с данными data из поддерева с корнем localRoot, выполняя
* при этом балансировку дерева, возвращает указатель на корень
* поддерева без удаленного узла */
template <typename baseType>
AVLNode<baseType>* AVLTree<baseType>::_remove(nodePtr localRoot,
                                              baseType key) {
    if(localRoot == nullptr)
        return nullptr;

    if (key < localRoot->data())
        localRoot->leftChild = _remove(localRoot->leftChild, key);
    else if (key > localRoot->data())
        localRoot->rightChild = _remove(localRoot->rightChild, key);
    else{
        nodePtr left = localRoot->leftChild, right = localRoot->rightChild;
        // Освобождаем память занимаемую узлом
        delete localRoot;

        // Правый потомок не существует
        if (right == nullptr)
            // Заменяем левым, левый тоже может быть пустым, в таком случае
            // удаляемый узел - листовой
            return left;

        // В противном случае находим приемника
        nodePtr min = _min(right);
        // "отцепляем" его присваем его правому потомку, то что вернет removeMin
        min->rightChild = removeMin(right);
        // Левому потомку присваиваем левого потомка удаленного
        min->leftChild = left;
        // Балансируем и вставляем на место удаленного
        return min->balance();
    }
    return localRoot->balance();
}

/* Удаляет узел с минимальным ключем из поддерева c корнем localRoot,
* выполняя баласировку дерева, возвращает указатель на правый узел
* удаленного узла */
template <typename baseType>
AVLNode<baseType>* AVLTree<baseType>::removeMin(nodePtr localRoot) {
    if (localRoot->leftChild == nullptr)
        return localRoot->rightChild;

    localRoot->leftChild = removeMin(localRoot->leftChild);
    return localRoot->balance();
}

// УДАЛЕНИЕ ДЕРЕВА

template <typename baseType>
AVLTree<baseType>::~AVLTree<baseType>() {
    deleteTree(root);
    root = nullptr;
}

template <typename baseType>
void AVLTree<baseType>::deleteTree(nodePtr localRoot) {
    if (localRoot != nullptr){
        deleteTree(localRoot->leftChild);
        deleteTree(localRoot->rightChild);
        delete localRoot;
    }
}

// Конец реализации АВЛ-дерева