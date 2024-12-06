

typedef struct set SET;
typedef struct student STUD;

STUD *createStud(int id, int age);
SET *createDataSet();
void destroyDataSet(SET *sp);
int numElements(SET *sp);
void insertStud(SET *sp, STUD *elt);
STUD **removeStud(SET *sp, int age, int *size);
STUD **searchAge(SET *sp, int age, int *size);
int maxAgeGap(SET *sp);
