# Davide Martinelli SM3201226

# Excpetion #

# An approach of handling exceptions at the outermost level has been chosen. 
# In short, when an error is handled, it results in a series of 1, 2, or 3 cascading exceptions, 
# allowing, during debugging, to track the program's execution and what it was processing.

# Exception for an empty stack.
class EmptyStackException(Exception):
    pass

# Exception for when the user enters incorrect input.
class WrongInputException(Exception):
    pass

# Exception for when the user is missing a parameter.
class MissingParameterException(Exception):
    pass

# Exception for when a variable has not been initialized in the environment.
class MissingVariableException(Exception):
    pass

# Exception for when a variable has been initialized in the environment but without a value.
class MissingVariableValueException(Exception):
    pass

# Program body #

# Class for the stack
class Stack:

    def __init__(self): # Costruzione di uno stack vuoto
        self.data = []

    def push(self, x):  # Aggiunge elementi allo stack (in coda)
        self.data.append(x) 

    def pop(self):      # estraggo un elemento dallo stack
        if self.data == []:  # Se non vi sono altri elementi, allora lancio un Empty Stack
            raise EmptyStackException
        res = self.data[-1]  # Altrimenti, prendo l'ultimo elemento (coda)
        self.data = self.data[0:-1] # Aggiorno: il contenuto dello stack ora saranno solo gli elementi da 0 a i
        return res # ritorno l'elemento estratto dallo stack 

    def is_empty(self):
        if len(self.data) != 0:
            return False
        else:
            return True

    def __str__(self):  # Stampa
        return " ".join([str(s) for s in self.data])

# Class that transforms the string into one or more objects that will then be executed.
class Expression:

    # I initialize the stack.
    def __init__(self):
        self.expression_stack = Stack()

    @classmethod
    def from_program(cls, text, dispatch):
        expr = cls()

        # I check the parameters.
        if type(text) is not str or type(dispatch) is not dict:
            raise WrongInputException
        
        # I split the string into a list of words.
        split_expression = text.split(" ") 

        # I iterate through the list.
        for i in split_expression:

            try:
                # If I can turn that word into an integer, I save it as a constant class.
                int_i = int(i)
                saved = Constant(int_i)

            except:
                # If it's a string not convertible to a number, I check if it's a known operation.
                if i in dispatch.keys(): # If it's in the dictionary.
                    args = [] # I initialize the list to be passed as a parameter.
                    current_operation = dispatch[i]            
                    K = current_operation.arity # I save the arity of that class.

                    try: # I enter a try block because the elements might not be enough.
                        for i in range(K): # For as many times as it requires elements.
                            element = expr.expression_stack.pop() # I remove them from the stack.
                            args.append(element) # And I put them in the list.
                        
                        # I can then push the result of that operation onto the stack.
                        saved = current_operation(args) # I simply pass the list of elements as a parameter.
                        
                    except:
                        raise MissingParameterException

                else:
                    # If the string is not an operation, then it is a variable.
                    saved = Variable(i)
        
            expr.expression_stack.push(saved) # I insert it into the stack.
            # In this way, I will read my operations in reverse, but the last operation...
            # ...that I will read will be the first to be removed from the stack.
    
        return expr # I return this instance that I have modified.

    def evaluate(self, env):

        # I base everything on recursive cascade. I will execute the evaluate function on all elements of the stack,
        # and, if the last operation returns a value, I make it return to the main function in turn.
        # Another important thing is that once all elements are popped, you also need to push.
        # Otherwise, the class becomes unusable, and every time I have to do the string from the program.
        # If the intention is, however, precisely this, just remove the push from the for loop.

        if type(env) is not dict:
            raise WrongInputException
        value = 0
        elem = []
        while not self.expression_stack.is_empty():
            i = self.expression_stack.pop()
            elem.append(i)
        for i in elem:
            value = i.evaluate(env)
            self.expression_stack.push(i)
        return value
        
    
    def __str__(self):

        # The __str__ def work as the same as evaluate. Excpete, it print the code.

        value = ""
        elem = []
        while not self.expression_stack.is_empty():
            i = self.expression_stack.pop()
            elem.append(i)
        for i in elem:
            value += i.__str__() + "\n"
            self.expression_stack.push(i)
        return value

# Class that associates words that are not operations or integers with variables.
class Variable(Expression):

    def __init__(self, name):
        self.name = name

    def evaluate(self, env):
        if self.name not in env:
            raise MissingVariableException
        elif env[self.name] == None:
            raise MissingVariableValueException
        else:
            # If I have reached this point, both the variable and the value are present. I return the value of this variable.
            return env[self.name]

    def __str__(self):
        return self.name

# Class that saves integers into an object.
class Constant(Expression):

    def __init__(self, value):
        self.value = value

    def evaluate(self, env):
        return self.value

    def __str__(self):
        return self.value

# Parent class that will then allow the implementation of more specific operations.
class Operation(Expression):

    def __init__(self, args):
        self.args = args # I save the list of arguments.

    def evaluate(self, env):
        # An evaluation of the operation occurs with the operation between
        # the two parts. However, it depends on whether they are directly accessible.
        return self.op(self.args, env) # I use the concept of cascading operations and execute them on the evaluate of the subclass.

    def op(self, *args):
        raise NotImplementedError()
        # It will be left to the specific subclasses.

    def __str__(self):
        pass

# Operations#

class NonaryOp(Operation):
    arity = 0 # arità della classe

class UnaryOp(Operation):
    arity = 1 # arità della classe

class BinaryOp(Operation):
    arity = 2

class TernaryOp(Operation):
    arity = 3

class QuaternaryOp(Operation):
    arity = 4

# Each of these classes assumes that errors have already been handled externally, 
# except for errors related to specific operations, which will be handled by internal Python exceptions. 
# I don't think I need to comment on the classes because they are very simple and intuitive, 
# and they perfectly follow the explanation provided in the PDF."

class Addition(BinaryOp):
    
    def op(self, args, env):
        
        return args[0].evaluate(env) + args[1].evaluate(env)
    
    def __str__(self):
        return f"({self.args[0].__str__()} + {self.args[1].__str__()})"

class Subtraction(BinaryOp):

    def op(self, args, env):
        
        return args[0].evaluate(env) - args[1].evaluate(env)
    
    def __str__(self):
        return f"({self.args[0].__str__()} - {self.args[1].__str__()})"

class Division(BinaryOp):
    
    def op(self, args, env):
        
        return args[0].evaluate(env) / args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0].__str__()} / {self.args[1].__str__()})"

class Multiplication(BinaryOp):

    def op(self, args, env):
        
        return args[0].evaluate(env) * args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0].__str__()} * {self.args[1].__str__()})"

class Modulus(BinaryOp):
    
    def op(self, args, env):
        return args[0].evaluate(env) % args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0].__str__()} % {self.args[1].__str__()})"

class Reciprocal(UnaryOp):
    
    def op(self, args, env):
        return 1/args[0].evaluate(env)

    def __str__(self):
        return f"(1/{self.args[0].__str__()})"

class AbsoluteValue(UnaryOp):
    
    def op(self, args, env):
        return abs(args[0].evaluate(env))

    def __str__(self):
        return f"|{self.args[0].__str__()}|"

class Power(BinaryOp):
    
    def op(self, args, env):
        return args[0].evaluate(env)**args[1].evaluate(env)

    def __str__(self):
        return f"pow({self.args[0].__str__()},{self.args[1].__str__()})"

class GratherThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) > args[1].evaluate(env):
            return 1
        return 0

    def __str__(self):
        return f"({self.args[0].__str__()} > {self.args[1].__str__()})"

class GratherEqualThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) >= args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} >= {self.args[1].__str__()})"
    
class LessThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) < args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} < {self.args[1].__str__()})"

class LessEqualThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) <= args[1].evaluate(env):
            return 1
        return 0

    def __str__(self):
        return f"({self.args[0].__str__()} <= {self.args[1].__str__()})"

class Equal(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) == args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} == {self.args[1].__str__()})"

class NotEqual(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) != args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} != {self.args[1].__str__()})"

class Print(UnaryOp):

    def op(self, args, env):
        x = args[0].evaluate(env)
        print(x)    
        return x 
    
    def __str__(self):
        return f"print {self.args[0].__str__()}"

class IF(TernaryOp):

    def op(self, args, env):
        if args[0].evaluate(env):
            return args[1].evaluate(env)
        else:
            return args[2].evaluate(env)

    def __str__(self):
        return f"if {self.args[0].__str__()}[\n  {self.args[1].__str__()}\n]else[\n  {self.args[2].__str__()}]"

class While(BinaryOp):
    
    def op(self, args, env):
        while args[0].evaluate(env):
            args[1].evaluate(env)

    def __str__(self):
        return f"while {self.args[0].__str__()}[\n{self.args[1].__str__()}]"

class For(QuaternaryOp):
    
    def op(self, args, env):
        if args[0] not in env:
            env[args[0]] = 0 
        elif env[args[0]] == None:
            raise MissingVariableException

        for i in range(args[1].evaluate(env),args[2].evaluate(env)):
            env[args[0].__str__()] = i
            args[3].evaluate(env)

    def __str__(self):
        return f"for {self.args[0].__str__()} in range({self.args[1].__str__()},{self.args[2].__str__()})[\n{self.args[3].__str__()}]"

class Setq(BinaryOp):

    def op(self, args, env):
        env[args[0].__str__()] = args[1].evaluate(env)

    def __str__(self):
        return f"set {self.args[0].__str__()} = {self.args[1].__str__()}"

class Setv(TernaryOp):

    def op(self, args, env):
        env[args[0].__str__()][args[1].evaluate(env)] = args[2].evaluate(env)

    def __str__(self):
        return f"set {self.args[0].__str__()}[{self.args[1].__str__()}] = {self.args[2].__str__()}"

class Alloc(UnaryOp):

    def op(self, args, env):
        env[args[0].__str__()] = 0

    def __str__(self):
        return f"alloc {self.args[0].__str__()} = 0"

class vAlloc(BinaryOp):
    
    def op(self, args, env):
        env[args[0].__str__()] = [0] * args[1].evaluate(env)

    def __str__(self):
        return f"valloc {self.args[0].__str__()}[{self.args[1].__str__()}] = [0]"

class Prog2(BinaryOp):

    def op(self, args, env):
        args[0].evaluate(env)
        return args[1].evaluate(env)

    def __str__(self):
        return f"prog2 [\n{self.args[0].__str__()}\n{self.args[1].__str__()}]"
    
class Prog3(TernaryOp):

    def op(self, args, env):
        args[0].evaluate(env)
        args[1].evaluate(env)
        return args[2].evaluate(env)

    def __str__(self):
        return f"prog3 [\n{self.args[0].__str__()}\n{self.args[1].__str__()}\n{self.args[2].__str__()}]"
    
class Prog4(QuaternaryOp):

    def op(self, args, env):
        args[0].evaluate(env)
        args[1].evaluate(env)
        args[2].evaluate(env)
        return args[3].evaluate(env)

    def __str__(self):
        return f"prog4 [\n{self.args[0].__str__()}\n{self.args[1].__str__()}\n{self.args[2].__str__()}\n{self.args[3].__str__()}]"
    
class Nop(NonaryOp):

    def op(self, args, env):
        pass

    def __str__(self):
        return "nop"   

class DefSub(BinaryOp):

    def op(self, args, env):
        if args[0] not in env:
            env[args[0].__str__()] = args[1].evaluate
        elif env[args[0]] == None:
            raise MissingVariableException 
        else:
            env[args[0].__str__()] = args[1].evaluate

    def __str__(self):
        return f"defsub {self.args[0].__str__()} = ({self.args[1].__str__()})"   

class Call(UnaryOp):

    def op(self, args, env):
        if args[0].__str__() not in env:
            raise MissingVariableException 
        elif env[args[0].__str__()] == None:
            raise MissingVariableException
        else:
            return env[args[0].__str__()](env)

    def __str__(self):
        return f"call {self.args[0].__str__()}"

def main():
    d = {"+" : Addition, "*" : Multiplication, "-" : Subtraction, "/" : Division, "%" : Modulus, "1/": Reciprocal, "abs": AbsoluteValue, "**": Power, 
        ">" : GratherThan, ">=" : GratherEqualThan, "<" : LessThan, "<=" : LessEqualThan, "=" : Equal, "!=" : NotEqual, 
        "print" : Print, "if" : IF, "while" : While, "for" : For, "setq" : Setq, "setv" : Setv, "alloc" : Alloc, "valloc" : vAlloc, 
        "prog2" : Prog2, "prog3" : Prog3, "prog4" : Prog4, "nop" : Nop, "defsub" : DefSub, "call" : Call}

    example = "x abs 2 ** -5 x setq x alloc prog3"

    e = Expression.from_program(example, d)
    print(e)
    res = e.evaluate(dict())
    print(res)

if __name__ == "__main__":
    main()
