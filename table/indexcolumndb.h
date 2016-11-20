#ifndef INDEXCOLUMNDB_H
#define INDEXCOLUMNDB_H


class indexColumnDb
{
public:
    int index_main_id;      //номер столбца main_id
    int index_name_of_pb;   //номер столбца названия пубикации
    int index_type_of_pb;   //номер столбца вида публикации
    int index_type_paper;   //номер столбца типа, рукописный или печатный
    int index_publish_hs;   //номер столбца издательства
    int index_year_pb;      //номер столбца года издания
    int index_numberpage;   //номер столбца страницы
    int index_pl;           //номер столбца пл
    int index_authors_pl;   //номер столбца Авторских пл
    int index_co_authors;   //номер столбца соавторов
    int index_secur_clas;   //номер столбца гриф. (security classification)
    int index_annotation;   //номер столбца примечания
    indexColumnDb();
};

#endif // INDEXCOLUMNDB_H
