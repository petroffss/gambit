//
// FILE: btableau.h:  Base Tableau classes  
//
// $Id$
//

#ifndef BTABLEAU_H  
#define BTABLEAU_H

#include "rational.h"
#include "bfs.h"
#include "basis.h"

// ---------------------------------------------------------------------------
//                          BaseTableau Stuff
// ---------------------------------------------------------------------------


template <class T> class BaseTableau {
public:
      // constructors and destructors
//  virtual ~BaseTableau();
  
      // information
  bool ColIndex(int) const;
  bool RowIndex(int) const;
  bool ValidIndex(int) const;
  virtual int MinRow() const = 0;
  virtual int MaxRow() const = 0;
  virtual int MinCol() const = 0;
  virtual int MaxCol() const = 0;

  virtual bool Member(int i) const = 0;
    // is variable i is a member of basis
  virtual int Label(int i) const = 0;   
    // return variable in i'th position of Tableau
  virtual int Find(int i) const = 0;  
    // return position of variable i
  
      // pivoting
  virtual int CanPivot(int outgoing,int incoming) = 0;
  virtual void Pivot(int outrow,int col) = 0;
      // perform pivot operation -- outgoing is row, incoming is column
  void CompPivot(int outlabel,int col);
  virtual long NumPivots() const = 0;
  
      // raw Tableau functions
  virtual  void Refactor() = 0;
};

// ---------------------------------------------------------------------------
//                           TableauInterface Stuff
// ---------------------------------------------------------------------------

template <class T> class TableauInterface : public BaseTableau<T>{
protected:
  const gMatrix<T> *A;  // should this be private?
  const gVector<T> *b;  // should this be private?
  Basis basis; 
  gVector<T> solution;  // current solution vector. should this be private?
  long npivots;
  T eps1,eps2;
public:
  TableauInterface(const gMatrix<T> &A, const gVector<T> &b); 
  TableauInterface(const TableauInterface<T>&);
  virtual ~TableauInterface();

  TableauInterface<T>& operator=(const TableauInterface<T>&);

  // information

  int MinRow() const;
  int MaxRow() const;
  int MinCol() const;
  int MaxCol() const;

  Basis & GetBasis(void);
  const gMatrix<T> & Get_A(void) const;
  const gVector<T> & Get_b(void) const;
  
  bool Member(int i) const;
  int Label(int i) const;   // return variable in i'th position of Tableau
  int Find(int i) const;  // return Tableau position of variable i

  long NumPivots() const;
  long &NumPivots();
  
  void Mark(int label);     // marks label to block it from entering basis
  void UnMark(int label);   // unmarks label
  bool IsBlocked(int label) const;   // returns true if label is blocked
  
  virtual void BasisVector(gVector<T> &x) const = 0; // solve M x = (*b)
  void GetColumn( int , gVector<T> &) const;  // raw column
  void GetBasis( Basis & ) const; // return Basis for current Tableau

  BFS<T> GetBFS1(void) const; 
  BFS<T> GetBFS(void);  // used in lpsolve for some reason
  void Dump(gOutput &) const;
  void BigDump(gOutput &);

  virtual int CanPivot(int outgoing,int incoming) = 0;
  virtual void Pivot(int outrow,int col) = 0; // pivot -- outgoing is row, incoming is column
  virtual void SolveColumn(int, gVector<T> &) = 0;  // column in new basis 
  virtual void Solve(const gVector<T> &b, gVector<T> &x) = 0;  // solve M x = b
  virtual void SolveT(const gVector<T> &c, gVector<T> &y) = 0;  // solve y M = c

  virtual void Refactor() = 0;
  virtual void SetRefactor(int) = 0;

      // miscellaneous functions
  bool EqZero(T x) const;
  bool LtZero(T x) const;
  bool GtZero(T x) const;
  bool LeZero(T x) const;
  bool GeZero(T x) const;
  T Epsilon(int i = 2) const;
};

#endif     // BTABLEAU_H