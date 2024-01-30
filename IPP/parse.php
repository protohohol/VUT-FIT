<?php
ini_set('display_errors', 'stderr');

/**
 * function to exit with error code and to display simple error messages.
 * 10 --- wrong argument
 * 21 --- missing header
 * 22 --- unknown instruction
 * 23 --- other syntax or lexical error
 */
function ex_er($err_code) {
    switch ($err_code) {
        case 10:
            echo("Wrong argument!\n");
            exit(10);
        case 21:
            echo("Missing header!\n");
            exit(21);
        case 22:
            echo("Unknown instruction!\n");
            exit(22);
        case 23:
            echo("Other synt or lex error!\n");
            exit(23);
    }
}

/**
 * class for token that contains variables for every argument, instruction name, counter for instructions and amount of arguments for every instruction
 * constructor takes one line and creates token from this
 */
class Token {
    public $instruction, $arg1, $arg2, $arg3;
    public $amount=0;
    public static $counter=0;

    function __construct($orig_input)
    {
        $this->create_token($orig_input);
    }

    /**
     * method to process variable with the help of regex
     */
    function process_var($var) {
        return preg_match("/\A(LF|GF|TF)@[a-zA-Z_\-$&%*!?]([a-zA-Z_\-$&%*!?0-9]|(&amp;))*\z/", $var);
    }

    /**
     * method to process symbol with the help of regex
     */
    function process_symb($symb) {
        return $this->process_var($symb) ? 1 : (preg_match("/\A(string@(\\\\[0-9]{3}|[^\\\\#\s])*|(int@(\-|\+)?[0-9]+)|(bool@(true|false))|(nil@nil))*\z/", $symb) ? 2 : 0);
    }

    /**
     * method to process label with the help of regex
     */
    function process_label($label) {
        return preg_match("/\A[a-zA-Z_\-$%*!?]([a-zA-Z_\-$&%*!?0-9]|(&amp;))*\z/", $label);
    }

    /**
     * method to process type with the help of regex
     */
    function process_type($type) {
        return preg_match("/^(string|int|bool|nil)$/", $type);
    }

    /**
     * method to format arguments to xml format (changes '&' with '&amp;', '<' with '&lt;' and '>' with '&gt;')
     */
    function format_xml() {
        if ($this->arg1 != null) {
            $this->arg1 = preg_replace("/&/","&amp;", $this->arg1);
            $this->arg1 = preg_replace("/</","&lt;", $this->arg1);
            $this->arg1 = preg_replace("/>/","&gt;", $this->arg1);
        }

        if ($this->arg2 != null) {
            $this->arg2 = preg_replace("/&/","&amp;", $this->arg2);
            $this->arg2 = preg_replace("/</","&lt;", $this->arg2);
            $this->arg2 = preg_replace("/>/","&gt;", $this->arg2);
        }
        
        if ($this->arg3 != null) {
            $this->arg3 = preg_replace("/&/","&amp;", $this->arg3);
            $this->arg3 = preg_replace("/</","&lt;", $this->arg3);
            $this->arg3 = preg_replace("/>/","&gt;", $this->arg3);
        }
    }

    function create_token($input) {
        if ($input != null) {
            $input = preg_replace("/#.*/", "", $input);
            if (preg_match("/.IPPcode23/", $input)) {
                ex_er(22);
            }
            $input = preg_replace("/[ ]+/", " ", $input);
        } else {
            return;
        }
        if ($input == "\n" || $input == "") {
            return;
        } else {
            $exp_input = explode(' ', trim($input, " \n"));
        }

        switch (strtoupper($exp_input[0])) {

            // no args
            case 'CREATEFRAME':
            case 'PUSHFRAME':
            case 'POPFRAME':
            case 'RETURN':
            case 'BREAK':
                if (sizeof($exp_input) != 1) {
                    ex_er(23);
                } else {
                    $this->amount = 0;
                    $this->instruction = $exp_input[0];
                }
                break;

            // <var>
            case 'DEFVAR':
            case 'POPS':
                if (sizeof($exp_input) != 2) {
                    ex_er(23);
                } else {
                    $this->amount = 1;
                    $this->instruction = $exp_input[0];
                    if ($this->process_var($exp_input[1])) {
                        $this->arg1[0] = "var";
                        $this->arg1[1] = $exp_input[1];
                    } else {
                        echo("<<".$exp_input[1].">>\n");
                        ex_er(23);
                    }
                }
                break;

            // <label>
            case 'CALL':
            case 'LABEL':
            case 'JUMP':
                if (sizeof($exp_input) != 2) {
                    ex_er(23);
                } else {
                    $this->amount = 1;
                    $this->instruction = $exp_input[0];
                    if ($this->process_label(($exp_input[1]))) {
                        $this->arg1[0] = "label";
                        $this->arg1[1] = $exp_input[1];
                    } else {
                        ex_er(23);
                    }
                }
                break;

            // <symb>
            case 'PUSHS':
            case 'WRITE':
            case 'EXIT':
            case 'DPRINT':
                if (sizeof($exp_input) != 2) {
                    ex_er(23);
                } else {
                    $this->amount = 1;
                    $this->instruction = $exp_input[0];
                    if ($this->process_symb($exp_input[1]) == 1) {
                        $this->arg1[0] = "var";
                        $this->arg1[1] = $exp_input[1];
                    } else if ($this->process_symb($exp_input[1]) == 2) {
                        $temp_arg = explode('@', $exp_input[1], 2);
                        $this->arg1[0] = $temp_arg[0];
                        $this->arg1[1] = $temp_arg[1];
                    } else {
                        ex_er(23);
                    }
                }
                break;

            // <var> <symb>
            case 'MOVE':
            case 'INT2CHAR':
            case 'STRLEN':
            case 'TYPE':
            case 'NOT':
                if (sizeof($exp_input) != 3) {
                    ex_er(23);
                } else {
                    $this->amount = 2;
                    $this->instruction = $exp_input[0];
                    if ($this->process_var($exp_input[1])) {
                        $this->arg1[0] = "var";
                        $this->arg1[1] = $exp_input[1];
                    } else {
                        ex_er(23);
                    }
                    if ($this->process_symb($exp_input[2]) == 1) {
                        $this->arg2[0] = "var";
                        $this->arg2[1] = $exp_input[2];
                    } else if ($this->process_symb($exp_input[2]) == 2) {
                        $temp_arg = explode('@', $exp_input[2], 2);
                        $this->arg2[0] = $temp_arg[0];
                        $this->arg2[1] = $temp_arg[1];
                    } else {
                        ex_er(23);
                    }
                }
                break;

            // <var> <type>
            case 'READ':
                if (sizeof($exp_input) != 3) {
                    ex_er(23);
                } else {
                    $this->amount = 2;
                    $this->instruction = $exp_input[0];
                    if ($this->process_var($exp_input[1])) {
                        $this->arg1[0] = "var";
                        $this->arg1[1] = $exp_input[1];
                    } else {
                        ex_er(23);
                    }
                    if ($this->process_type($exp_input[2])) {
                        $this->arg2[0] = "type";
                        $this->arg2[1] = $exp_input[2];
                    } else {
                        ex_er(23);
                    }
                }
                break;

            // <var> <symb1> <symb2>
            case 'ADD':
            case 'SUB':
            case 'MUL':
            case 'IDIV':
            case 'LT':
            case 'GT':
            case 'EQ':
            case 'AND':
            case 'OR':
            case 'STRI2INT':
            case 'CONCAT':
            case 'GETCHAR':
            case 'SETCHAR':
                if (sizeof($exp_input) != 4) {
                    ex_er(23);
                } else {
                    $this->amount = 3;
                    $this->instruction = $exp_input[0];
                    if ($this->process_var($exp_input[1])) {
                        $this->arg1[0] = "var";
                        $this->arg1[1] = $exp_input[1];
                    } else {
                        ex_er(23);
                    }
                    if ($this->process_symb($exp_input[2]) == 1) {
                        $this->arg2[0] = "var";
                        $this->arg2[1] = $exp_input[2];
                    } else if ($this->process_symb($exp_input[2]) == 2) {
                        $temp_arg = explode('@', $exp_input[2], 2);
                        $this->arg2[0] = $temp_arg[0];
                        $this->arg2[1] = $temp_arg[1];
                    } else {
                        ex_er(23);
                    }
                    if ($this->process_symb($exp_input[3]) == 1) {
                        $this->arg3[0] = "var";
                        $this->arg3[1] = $exp_input[3];
                    } else if ($this->process_symb($exp_input[3]) == 2) {
                        $temp_arg = explode('@', $exp_input[3], 2);
                        $this->arg3[0] = $temp_arg[0];
                        $this->arg3[1] = $temp_arg[1];
                    } else {
                        ex_er(23);
                    }
                }
                break;

            // <label> <symb1> <symb2>
            case 'JUMPIFEQ':
            case 'JUMPIFNEQ':
                if (sizeof($exp_input) != 4) {
                    ex_er(23);
                } else {
                    $this->amount = 3;
                    $this->instruction = $exp_input[0];
                    if ($this->process_label(($exp_input[1]))) {
                        $this->arg1[0] = "label";
                        $this->arg1[1] = $exp_input[1];
                    } else {
                        ex_er(23);
                    }
                    if ($this->process_symb($exp_input[2]) == 1) {
                        $this->arg2[0] = "var";
                        $this->arg2[1] = $exp_input[2];
                    } else if ($this->process_symb($exp_input[2]) == 2) {
                        $temp_arg = explode('@', $exp_input[2], 2);
                        $this->arg2[0] = $temp_arg[0];
                        $this->arg2[1] = $temp_arg[1];
                    } else {
                        ex_er(23);
                    }
                    if ($this->process_symb($exp_input[3]) == 1) {
                        $this->arg3[0] = "var";
                        $this->arg3[1] = $exp_input[3];
                    } else if ($this->process_symb($exp_input[3]) == 2) {
                        $temp_arg = explode('@', $exp_input[3], 2);
                        $this->arg3[0] = $temp_arg[0];
                        $this->arg3[1] = $temp_arg[1];
                    } else {
                        ex_er(23);
                    }
                }
                break;
            default:
                ex_er(22);
        }
    }

    /**
     * method to generate output in XML format
     */
    function generate() {
        self::$counter++;
        echo("\t<instruction order=\"".self::$counter."\" opcode=\"".strtoupper($this->instruction)."\">\n");
        $this->format_xml();
        switch ($this->amount) {
            case 0:
                echo("\t</instruction>\n");
                break;
            case 1:
                echo("\t\t<arg1 type=\"".$this->arg1[0]."\">");
                if (sizeof($this->arg1) == 1) {
                    $this->arg1[1] = "";
                }
                echo($this->arg1[1]);
                echo("</arg1>\n");
                echo("\t</instruction>\n");
                break;
            case 2:
                echo("\t\t<arg1 type=\"".$this->arg1[0]."\">");
                if (sizeof($this->arg1) == 1) {
                    $this->arg1[1] = "";
                }
                echo($this->arg1[1]);
                echo("</arg1>\n");
                echo("\t\t<arg2 type=\"".$this->arg2[0]."\">");
                if (sizeof($this->arg2) == 1) {
                    $this->arg2[1] = "";
                }
                echo($this->arg2[1]);
                echo("</arg2>\n");
                echo("\t</instruction>\n");
                break;
            case 3:
                echo("\t\t<arg1 type=\"".$this->arg1[0]."\">");
                if (sizeof($this->arg1) == 1) {
                    $this->arg1[1] = "";
                }
                echo($this->arg1[1]);
                echo("</arg1>\n");
                echo("\t\t<arg2 type=\"".$this->arg2[0]."\">");
                if (sizeof($this->arg2) == 1) {
                    $this->arg2[1] = "";
                }
                echo($this->arg2[1]);
                echo("</arg2>\n");
                echo("\t\t<arg3 type=\"".$this->arg3[0]."\">");
                if (sizeof($this->arg3) == 1) {
                    $this->arg3[1] = "";
                }
                echo($this->arg3[1]);
                echo("</arg3>\n");
                echo("\t</instruction>\n");
                break;

        }
    }

}

// checks if user asks for help
if ($argc > 1) {
    if ($argv[1] == "--help" && $argc == 2) {
        echo("Usage : parser.php [options] <inputFile\n");
        exit(0);
    } else {
        ex_er(10);
    }
}

// delete all comments, whitespaces and skip empty lines
do {
    $input = fgets(STDIN);
    $input = preg_replace("/#.*/", "", $input);
    $input = preg_replace("/\s/", "", $input);
} while ($input == "");

// check if header is correct
if ($input != ".IPPcode23") {
    ex_er(21);
}

echo("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<program language=\"IPPcode23\">\n");

while (($input = fgets(STDIN))) {
    $t = new Token($input);
    if ($t->instruction != null && $t->instruction != "\n") {
        $t->generate();
    }
}

echo("</program>");

exit(0);
?>