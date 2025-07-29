import Dashboard from "./pages/Dashboard";

function App() {
  return (
    <div className="min-h-screen bg-gray-100 text-gray-800">
      <header className="bg-indigo-600 text-white p-4 text-xl font-bold">
        Whale Watch Dashboard
      </header>
      <main className="p-6">
        <Dashboard />
      </main>
    </div>
  );
}

export default App;
